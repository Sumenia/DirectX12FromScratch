#include <DirectXMath.h>
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12Device.h"
#include "MiniEngine/D3D12/D3D12ConstantBuffer.h"
#include "MiniEngine/D3D12/D3D12Material.h"
#include "MiniEngine/D3D12/D3D12Texture.h"

using namespace MiniEngine;

std::vector<UINT8> GenerateTextureData(unsigned int width, unsigned int height)
{
	const UINT rowPitch = width * 4;
	const UINT cellPitch = rowPitch >> 3;		// The width of a cell in the checkboard texture.
	const UINT cellHeight = height >> 3;	// The height of a cell in the checkerboard texture.
	const UINT textureSize = rowPitch * height;

	std::vector<UINT8> data(textureSize);
	UINT8* pData = &data[0];

	for (UINT n = 0; n < textureSize; n += 4)
	{
		UINT x = n % rowPitch;
		UINT y = n / rowPitch;
		UINT i = x / cellPitch;
		UINT j = y / cellHeight;

		if (i % 2 == j % 2)
		{
			pData[n] = 0x00;		// R
			pData[n + 1] = 0x00;	// G
			pData[n + 2] = 0x00;	// B
			pData[n + 3] = 0xff;	// A
		}
		else
		{
			pData[n] = 0xff;		// R
			pData[n + 1] = 0xff;	// G
			pData[n + 2] = 0xff;	// B
			pData[n + 3] = 0xff;	// A
		}
	}

	return data;
}

D3D12Material::D3D12Material(D3D12RenderSystem &system) : _system(system), _material(nullptr)
{}

D3D12Material::~D3D12Material()
{
    delete _material;
    _material = nullptr;
}

bool D3D12Material::bind(CommandList &list, unsigned int rootIdx)
{
    if (!_material)
        return (false);

	ID3D12DescriptorHeap* ppHeaps[] = { _cbvSrvDescHeap->getNative() };
	dynamic_cast<D3D12CommandList&>(list).getNative()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	dynamic_cast<D3D12CommandList&>(list).getNative()->SetGraphicsRootDescriptorTable(4, _cbvSrvDescHeap->getNative()->GetGPUDescriptorHandleForHeapStart());

	return (_material->bind(list, rootIdx));
}

bool D3D12Material::finalize()
{
    DWORD32 id = _id;
    size_t  cursor = 0;
    size_t  size = 0;
    char    *materialData = nullptr;

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

    if (!(_flags & Material::NORMAL_COLOR) && !(_flags & Material::TEXTURE_DIFFUSE))
    {
        DirectX::XMFLOAT3 color(_kd.x, _kd.y, _kd.z);
        pad(size, cursor, materialData, &color, sizeof(color));
    }

    if (!(_flags & Material::TEXTURE_SPECULAR))
    {
        DirectX::XMFLOAT3 color(_ks.x, _ks.y, _ks.z);
        pad(size, cursor, materialData, &color, sizeof(color));
    }

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
	
	if (!initCbvSrvDescriptorHeap())
		return (false);
	
	// Copy data to the intermediate upload heap and then schedule a copy 
	// from the upload heap to the Texture2D.
	std::vector<UINT8> texture = GenerateTextureData(128, 128);

	CD3DX12_CPU_DESCRIPTOR_HANDLE handle(_cbvSrvDescHeap->getNative()->GetCPUDescriptorHandleForHeapStart());
	D3D12Texture *tex;
	// D3D12Texture PART
	// Diffuse
	for (unsigned int i = 0; i < 3 /*nb TextureType*/; i++)
	{
		if (_textures[static_cast<TextureType>(i)] != nullptr)
		{
			tex = new D3D12Texture(_system);
			// TO-DO init texture with real data
			tex->init(_textures[static_cast<TextureType>(i)]->getImage()->getData(),
						_textures[static_cast<TextureType>(i)]->getImage()->getWidth(),
						_textures[static_cast<TextureType>(i)]->getImage()->getHeight());

			// Describe and create a SRV for the texture.
			D3D12_SHADER_RESOURCE_VIEW_DESC		srvDesc = {};
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.Format = tex->getResourceDesc().Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			_system.getDevice()->getNative()->CreateShaderResourceView(tex->getBuffer(), &srvDesc, handle);
		}
		handle.Offset(_cbvSrvDescHeap->getSize());
	}
    return (true);
}

bool D3D12Material::initCbvSrvDescriptorHeap()
{
	// Descriptor Heap for Textures SRVs
	_cbvSrvDescHeap = new D3D12DescriptorHeap(_system);
	return (_cbvSrvDescHeap->init(3, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE));
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