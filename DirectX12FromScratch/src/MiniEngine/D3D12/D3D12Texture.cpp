
#include "MiniEngine/D3D12/D3D12Device.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12Texture.h"

MiniEngine::D3D12Texture::D3D12Texture(D3D12RenderSystem & system) : _system(system)
{
}

MiniEngine::D3D12Texture::~D3D12Texture()
{
}

bool MiniEngine::D3D12Texture::init(void *data,
									unsigned int width,
									unsigned int height)
{
	HRESULT                     result;
	ID3D12Resource				*bufferUpload;

	// init CommandList
	D3D12CommandList *commandList = _system.getCommandQueue()->createCommandList(nullptr);
	commandList->init();

	// Describe and create a Texture2D.
	_textureDesc.MipLevels = 1;
	_textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	_textureDesc.Width = width;
	_textureDesc.Height = height;
	_textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	_textureDesc.DepthOrArraySize = 1;
	_textureDesc.SampleDesc.Count = 1;
	_textureDesc.SampleDesc.Quality = 0;
	_textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	result = _system.getDevice()->getNative()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&_textureDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		__uuidof(ID3D12Resource),
		(void**)&_buffer);

	const UINT64 uploadBufferSize = GetRequiredIntermediateSize(_buffer, 0, 1);

	// Create the GPU upload buffer.
	result = _system.getDevice()->getNative()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		__uuidof(ID3D12Resource),
		(void**)&bufferUpload);

	D3D12_SUBRESOURCE_DATA textureData = {};
	textureData.pData = data;
	textureData.RowPitch = width * 4;
	textureData.SlicePitch = textureData.RowPitch * height;

	if (!commandList->reset())
		return (false);

	UpdateSubresources(commandList->getNative(), _buffer, bufferUpload, 0, 0, 1, &textureData);
	commandList->getNative()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(_buffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

	if (!commandList->end())
		return (false);

	// Execute the list of commands.
	_system.getCommandQueue()->executeCommandLists(1, commandList);
	if (!_system.getCommandQueue()->wait(*_system.getFence()))
		return (false);
	return (true);
}

ID3D12Resource * MiniEngine::D3D12Texture::getBuffer()
{
	return (_buffer);
}

D3D12_RESOURCE_DESC & MiniEngine::D3D12Texture::getResourceDesc()
{
	return (_textureDesc);
}
