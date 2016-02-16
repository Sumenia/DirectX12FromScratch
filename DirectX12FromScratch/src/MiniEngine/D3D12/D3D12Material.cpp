#include <DirectXMath.h>
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12Device.h"
#include "MiniEngine/D3D12/D3D12ConstantBuffer.h"
#include "MiniEngine/D3D12/D3D12Material.h"
#include "MiniEngine/D3D12/D3D12Texture.h"

using namespace MiniEngine;

D3D12Material::D3D12Material(D3D12RenderSystem &system) : Material(system), _system(system), _material(nullptr), _cbvSrvDescHeap(nullptr)
{}

D3D12Material::~D3D12Material()
{
    delete _material;
    _material = nullptr;

    delete _cbvSrvDescHeap;
    _cbvSrvDescHeap = nullptr;
}

bool D3D12Material::bind(CommandList &list, unsigned int rootIdx)
{
    if (!_material)
        return (false);
    
    if (!_material->bind(list, rootIdx))
        return (false);

    if (_cbvSrvDescHeap)
    {
        ID3D12DescriptorHeap* ppHeaps[] = { _cbvSrvDescHeap->getNative() };

        dynamic_cast<D3D12CommandList&>(list).getNative()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
        dynamic_cast<D3D12CommandList&>(list).getNative()->SetGraphicsRootDescriptorTable(4, _cbvSrvDescHeap->getNative()->GetGPUDescriptorHandleForHeapStart());

    }
	
	return (true);
}

bool D3D12Material::finalize()
{
    DWORD32         id = _id;
    size_t          cursor = 0;
    size_t          size = 0;
    char            *materialData = nullptr;
    unsigned int    nbSrvs = 0;

    // Get size of material struct
    padSize(size, sizeof(id));

    if (!(_flags & Material::TEXTURE_AMBIENT))
        padSize(size, sizeof(DirectX::XMFLOAT3));

    if (!(_flags & Material::NORMAL_COLOR) && !(_flags & Material::TEXTURE_DIFFUSE))
        padSize(size, sizeof(DirectX::XMFLOAT3));

    if (!(_flags & Material::TEXTURE_SPECULAR))
        padSize(size, sizeof(DirectX::XMFLOAT3));

    padSize(size, sizeof(float));

    // Allocate material structure
    materialData = new char[size];

    // Fill material structure
    pad(size, cursor, materialData, &id, sizeof(id));

    if (!(_flags & Material::TEXTURE_AMBIENT))
    {
        DirectX::XMFLOAT3 color(_ka.x, _ka.y, _ka.z);
        pad(size, cursor, materialData, &color, sizeof(color));
    }
    else
        nbSrvs++;

    if (!(_flags & Material::NORMAL_COLOR) && !(_flags & Material::TEXTURE_DIFFUSE))
    {
        DirectX::XMFLOAT3 color(_kd.x, _kd.y, _kd.z);
        pad(size, cursor, materialData, &color, sizeof(color));
    }
    else if (!(_flags & Material::NORMAL_COLOR))
        nbSrvs++;

    if (!(_flags & Material::TEXTURE_SPECULAR))
    {
        DirectX::XMFLOAT3 color(_ks.x, _ks.y, _ks.z);
        pad(size, cursor, materialData, &color, sizeof(color));
    }
    else
        nbSrvs++;

    if (_flags & Material::NORMAL_MAP)
        nbSrvs++;

    pad(size, cursor, materialData, &_shininess, sizeof(float));

    // Create constant buffer
    _material = new D3D12ConstantBuffer(_system);

    if (!_material->init(size))
    {
        delete[] materialData;
        return (false);
    }

    if (!_material->update(size, materialData))
    {
        delete[] materialData;
        return (false);
    }

    delete[] materialData;
	
    // Init SRVs
    
    if (nbSrvs > 0)
    {
        if (!initCbvSrvDescriptorHeap(nbSrvs))
            return (false);

        CD3DX12_CPU_DESCRIPTOR_HANDLE handle(_cbvSrvDescHeap->getNative()->GetCPUDescriptorHandleForHeapStart());

        for (unsigned int i = 0; i < 4 /* TO-DO: Apply by type */; i++)
        {
            if (_textures[static_cast<TextureType>(i)] != nullptr)
            {
                D3D12Texture* tex = dynamic_cast<D3D12Texture*>(_textures[static_cast<TextureType>(i)]);

                // Describe and create a SRV for the texture.
                D3D12_SHADER_RESOURCE_VIEW_DESC		srvDesc = {};
                srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                srvDesc.Format = tex->getResourceDesc().Format;
                srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
                srvDesc.Texture2D.MipLevels = 1;

                _system.getDevice()->getNative()->CreateShaderResourceView(tex->getBuffer(), &srvDesc, handle);
                handle.Offset(_cbvSrvDescHeap->getSize());
            }
        }
    }

    return (true);
}

bool D3D12Material::initCbvSrvDescriptorHeap(unsigned int nbSrvs)
{
	// Descriptor Heap for Textures SRVs
	_cbvSrvDescHeap = new D3D12DescriptorHeap(_system);
	return (_cbvSrvDescHeap->init(nbSrvs, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE));
}

void D3D12Material::padSize(size_t &size, size_t sizeData)
{
    if ((size + sizeData) % 16 == 0 || size / 16 == (size + sizeData) / 16)
    {
        size += sizeData;
        return;
    }

    size = ((size + 16) / 16) * 16;
    size += sizeData;
}

void D3D12Material::pad(size_t size, size_t &cursor, char *materialData, void *data, size_t sizeData)
{
    if ((cursor + sizeData) % 16 != 0 && cursor / 16 != (cursor + sizeData) / 16)
        cursor = ((cursor + 16) / 16) * 16;

    memcpy(materialData + cursor, data, sizeData);
    cursor += sizeData;
}