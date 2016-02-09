#include "MiniEngine/Light.h"
#include "MiniEngine/D3D12/D3D12ConstantBuffer.h"
#include "MiniEngine/D3D12/D3D12Device.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"

using namespace MiniEngine;

D3D12ConstantBuffer::D3D12ConstantBuffer(D3D12RenderSystem &system) : ConstantBuffer(system), _system(system), _constantBuffer(nullptr), _nb(0), _size(0), _idx(0), _data(nullptr)
{
    for (auto &&needUpdate : _needUpdate)
        needUpdate = false;
}

D3D12ConstantBuffer::~D3D12ConstantBuffer()
{
    delete _data;
    _data = nullptr;

    if (_constantBuffer)
        for (unsigned int i = 0; i < _nb; i++)
            if (_constantBuffer[i])
                _constantBuffer[i]->Release();
    
    delete _constantBuffer;
    _constantBuffer = nullptr;
}


bool D3D12ConstantBuffer::init(unsigned int size, unsigned int nb)
{
    _nb = nb;
    _size = size;

    size = (size + 255) & ~255;

    return (
        initRessources(size, nb)
        && initData(size)
    );
}

bool D3D12ConstantBuffer::initRessources(unsigned int size, unsigned int nb)
{
    CD3DX12_HEAP_PROPERTIES     uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);

    _constantBuffer = new ID3D12Resource*[nb];

    ZeroMemory(_constantBuffer, sizeof(ID3D12Resource*) * nb);

    for (unsigned int i = 0; i < nb; i++)
    {
        CD3DX12_RESOURCE_DESC       constantBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(size);
        HRESULT                     result;

        result = _system.getDevice()->getNative()->CreateCommittedResource(
            &uploadHeapProperties,
            D3D12_HEAP_FLAG_NONE,
            &constantBufferDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            __uuidof(ID3D12Resource),
            (void**)&_constantBuffer[i]
        );

        if (FAILED(result))
            return (false);
    }

    return (true);
}

bool D3D12ConstantBuffer::initData(unsigned int size)
{
    _data = new char[size];
    return (!!_data);
}

bool D3D12ConstantBuffer::update(unsigned int size, void *data)
{
    CD3DX12_RANGE   readRange(0, 0);
    UINT8           *mappedMemory = nullptr;
    HRESULT         result;

    if (size > _size)
        return (false);

    if (data != _data)
        memcpy(_data, data, size);

    result = _constantBuffer[_idx]->Map(0, &readRange, reinterpret_cast<void**>(&mappedMemory));

    if (FAILED(result))
        return (false);

    memcpy(mappedMemory, data, size);

    _constantBuffer[_idx]->Unmap(0, &readRange);

    for (unsigned int i = 0; i < _nb; i++)
    {
        if (i != _idx)
            _needUpdate[i] = true;
    }

    return (true);
}

bool D3D12ConstantBuffer::bind(CommandList &commandList, unsigned int rootIdx)
{
    if (_needUpdate[_idx])
    {
        _needUpdate[_idx] = false;
        
        if (!update(_size, _data))
            return (false);
    }

    dynamic_cast<D3D12CommandList&>(commandList).getNative()->SetGraphicsRootConstantBufferView(rootIdx, _constantBuffer[_idx]->GetGPUVirtualAddress());

    _idx = (_idx + 1) % _nb;

    return (true);
}

bool D3D12ConstantBuffer::updateCameraMatrix(Vector3f const &position, Matrix4f const &view, Matrix4f const &projection, unsigned int nb_lights)
{
    struct  CameraMatrix
    {
        DirectX::XMFLOAT3   position;
        float               pad1[1];
        DirectX::XMFLOAT4X4 view;
        DirectX::XMFLOAT4X4 projection;
        unsigned int        nb_lights;
    }       camera;

    for (unsigned int x = 0; x < 4; x++)
        for (unsigned int y = 0; y < 4; y++)
            camera.view.m[x][y] = view(x + 1, y + 1);

    for (unsigned int x = 0; x < 4; x++)
        for (unsigned int y = 0; y < 4; y++)
            camera.projection.m[x][y] = projection(x + 1, y + 1);

    camera.position = {};
    camera.nb_lights = nb_lights;

    return (update(sizeof(camera), &camera));
}

bool D3D12ConstantBuffer::updateModelMatrix(Matrix4f const &data)
{
    struct  ModelMatrix
    {
        DirectX::XMFLOAT4X4 model;
        DirectX::XMFLOAT4X4 modelNormal;
    }       model;

    Matrix4f    modelNormal = data.inverse().transpose();

    for (unsigned int x = 0; x < 4; x++)
        for (unsigned int y = 0; y < 4; y++)
            model.model.m[x][y] = data(x + 1, y + 1);

    for (unsigned int x = 0; x < 4; x++)
        for (unsigned int y = 0; y < 4; y++)
            model.modelNormal.m[x][y] = modelNormal(x + 1, y + 1);

    return (update(sizeof(model), &model));
}

bool D3D12ConstantBuffer::updateLights(std::list<Light*> &lights)
{
    struct  LightStructure
    {
        unsigned int        type;

        DirectX::XMFLOAT3   position;
        DirectX::XMFLOAT3   direction;

        float               cutOff;
        float               outerCutOff;
		

        DirectX::XMFLOAT3   ambient;
        DirectX::XMFLOAT3   diffuse;
        float               pad1[1];

        DirectX::XMFLOAT3   specular;

        float               constant;
        float               linear;
        float               quadratic;

        float               range;

        float               pad2[1];
    };

    LightStructure  lightsData[MAX_LIGHTS];

    // TO-DO: Pass all parameters for lights
    unsigned int i = 0;
    for (auto &&light : lights)
    {
        Vector3f    position = light->getParent()->getDerivedPosition();
        //Vector4f    direction(light->getDirection().x, light->getDirection().y, light->getDirection().z, 0.0f);

        lightsData[i].type = light->getType();
        lightsData[i].position = { position.x, position.y, position.z };
        lightsData[i].ambient = { light->getAmbient().x, light->getAmbient().y, light->getAmbient().z };
        lightsData[i].diffuse = { light->getDiffuse().x, light->getDiffuse().y, light->getDiffuse().z };
        lightsData[i].specular = { light->getSpecular().x, light->getSpecular().y, light->getSpecular().z };
        lightsData[i].cutOff = light->getInnerCutOff();
        lightsData[i].outerCutOff = light->getOuterCutOff();
        lightsData[i].direction = { light->getDirection().x, light->getDirection().y, light->getDirection().z };
        lightsData[i].quadratic = light->getQuadratic();
        lightsData[i].constant = light->getConstant();
        lightsData[i].linear = light->getLinear();
        lightsData[i].cutOff = light->getCosineLightAngle();
        lightsData[i].outerCutOff = light->getCosineBigAngle();
        lightsData[i].range = light->getRange();

        i++;
    }

    return (update(sizeof(lightsData), &lightsData));
}

ID3D12Resource *D3D12ConstantBuffer::getNative()
{
    return (_constantBuffer[_idx]);
}