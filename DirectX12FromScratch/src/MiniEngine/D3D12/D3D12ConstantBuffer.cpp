#include "MiniEngine/D3D12/D3D12ConstantBuffer.h"
#include "MiniEngine/D3D12/D3D12Device.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"

using namespace MiniEngine;

D3D12ConstantBuffer::D3D12ConstantBuffer(D3D12RenderSystem &system) : ConstantBuffer(system), _system(system), _constantBuffer(nullptr), _nb(0), _size(0), _idx(0), _type(NONE)
{
    for (auto &&needUpdate : _needUpdate)
        needUpdate = false;
}

D3D12ConstantBuffer::~D3D12ConstantBuffer()
{
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

bool D3D12ConstantBuffer::update(unsigned int size, void *data)
{
    CD3DX12_RANGE   readRange(0, 0);
    UINT8           *mappedMemory = nullptr;
    HRESULT         result;

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

        if (_type == CAMERA)
        {
            if (!update(sizeof(_camera), &_camera))
                return (false);
        }
        else if (_type == MODEL)
        {
            if (!update(sizeof(_model), &_model))
                return (false);
        }
    }

    dynamic_cast<D3D12CommandList&>(commandList).getNative()->SetGraphicsRootConstantBufferView(rootIdx, _constantBuffer[_idx]->GetGPUVirtualAddress());

    _idx = (_idx + 1) % _nb;

    return (true);
}

bool D3D12ConstantBuffer::updateCameraMatrix(Matrix4f const &view, Matrix4f const &projection)
{
    for (unsigned int x = 0; x < 4; x++)
        for (unsigned int y = 0; y < 4; y++)
            _camera.view.m[x][y] = view(x + 1, y + 1);

    for (unsigned int x = 0; x < 4; x++)
        for (unsigned int y = 0; y < 4; y++)
            _camera.projection.m[x][y] = projection(x + 1, y + 1);

    _type = CAMERA;
    return (update(sizeof(_camera), &_camera));
}

bool D3D12ConstantBuffer::updateModelMatrix(Matrix4f const &model)
{
    for (unsigned int x = 0; x < 4; x++)
        for (unsigned int y = 0; y < 4; y++)
            _model.model.m[x][y] = model(x + 1, y + 1);

    _type = MODEL;
    return (update(sizeof(_model), &_model));
}

ID3D12Resource *D3D12ConstantBuffer::getNative()
{
    return (_constantBuffer[_idx]);
}