#include <DirectXMath.h>
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12Device.h"
#include "MiniEngine/D3D12/D3D12ConstantBuffer.h"
#include "MiniEngine/D3D12/D3D12Material.h"

using namespace MiniEngine;

std::vector<UINT8> GenerateTextureData()
{
	const UINT rowPitch = 64 * 4;
	const UINT cellPitch = rowPitch >> 3;		// The width of a cell in the checkboard texture.
	const UINT cellHeight = 64 >> 3;	// The height of a cell in the checkerboard texture.
	const UINT textureSize = rowPitch * 64;

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
	_commandList->getNative()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	_commandList->getNative()->SetGraphicsRootDescriptorTable(4, _cbvSrvDescHeap->getNative()->GetGPUDescriptorHandleForHeapStart());

	return (_material->bind(list, rootIdx));
}

bool D3D12Material::finalize()
{
    DWORD32 id = _id;
    size_t  cursor = 0;
    size_t  size = 0;
    char    *materialData = nullptr;

	// init CommandList
	_commandList = _system.getCommandQueue()->createCommandList(nullptr);
	_commandList->init();

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
	
	HRESULT                     result;

	ID3D12Resource				*buffer;
	ID3D12Resource				*bufferUpload;

	// Describe and create a Texture2D.
	D3D12_RESOURCE_DESC textureDesc = {};
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.Width = 64;
	textureDesc.Height = 64;
	textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	result = _system.getDevice()->getNative()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		__uuidof(ID3D12Resource),
		(void**)&buffer);

	const UINT64 uploadBufferSize = GetRequiredIntermediateSize(buffer, 0, 1);

	// Create the GPU upload buffer.
	result = _system.getDevice()->getNative()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		__uuidof(ID3D12Resource),
		(void**)&bufferUpload);

	// Copy data to the intermediate upload heap and then schedule a copy 
	// from the upload heap to the Texture2D.
	std::vector<UINT8> texture = GenerateTextureData();

	D3D12_SUBRESOURCE_DATA textureData = {};
	textureData.pData = &texture[0];
	textureData.RowPitch = 64 * 4;
	textureData.SlicePitch = textureData.RowPitch * 64;

	if (!_commandList->reset())
		return (false);

	UpdateSubresources(_commandList->getNative(), buffer, bufferUpload, 0, 0, 1, &textureData);
	_commandList->getNative()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(buffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

	if (!_commandList->end())
		return (false);

	// Execute the list of commands.
	_system.getCommandQueue()->executeCommandLists(1, _commandList);
	if (!_system.getCommandQueue()->wait(*_system.getFence()))
		return (false);

	// Describe and create a SRV for the texture.
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	_system.getDevice()->getNative()->CreateShaderResourceView(buffer, &srvDesc, _cbvSrvDescHeap->getNative()->GetCPUDescriptorHandleForHeapStart());
    return (true);
}

bool D3D12Material::initCbvSrvDescriptorHeap()
{
	// Descriptor Heap for Material CBV + Textures SRVs
	_cbvSrvDescHeap = new D3D12DescriptorHeap(_system);
	return (_cbvSrvDescHeap->init(1, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE));
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