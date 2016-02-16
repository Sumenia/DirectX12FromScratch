
#include "MiniEngine/D3D12/D3D12Device.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12Texture.h"

using namespace MiniEngine;

D3D12Texture::D3D12Texture(D3D12RenderSystem &system) : _system(system), _buffer(nullptr)
{}

D3D12Texture::~D3D12Texture()
{
    if (_buffer)
        _buffer->Release();

    _buffer = nullptr;
}

bool D3D12Texture::loadFromFile(const std::string &filename, DXGI_FORMAT format)
{
    if (!Texture::loadFromFile(filename))
        return (false);

    return (init(_loader->getData(), _loader->getWidth(), _loader->getHeight(), format));
}

bool D3D12Texture::init(void *data, unsigned int width, unsigned int height, DXGI_FORMAT format)
{
	HRESULT                     result;
	ID3D12Resource				*bufferUpload;

	// init CommandList
	std::shared_ptr<D3D12CommandList> commandList(_system.getCommandQueue()->createCommandList(nullptr));

	if (!commandList->init())
		return (false);

	// Describe and create a Texture2D.
	ZeroMemory(&_textureDesc, sizeof(_textureDesc));
	_textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, width, height);

	result = _system.getDevice()->getNative()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&_textureDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		__uuidof(ID3D12Resource),
		(void**)&_buffer);

	if (_buffer == nullptr)
		return (false);

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

	if (bufferUpload == nullptr)
		return (false);

	D3D12_SUBRESOURCE_DATA textureData = {};
	textureData.pData = data;
    textureData.RowPitch = width * 3 * 4; // 4
	textureData.SlicePitch = textureData.RowPitch * height;

	if (!commandList->reset())
	{
		bufferUpload->Release();
		return (false);
	}

	UpdateSubresources(commandList->getNative(), _buffer, bufferUpload, 0, 0, 1, &textureData);
	commandList->getNative()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(_buffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

	if (!commandList->end())
	{
		bufferUpload->Release();
		return (false);
	}

	// Execute the list of commands.
	_system.getCommandQueue()->executeCommandLists(1, commandList.get());

	if (!_system.getCommandQueue()->wait(*_system.getFence()))
	{
		bufferUpload->Release();
		return (false);
	}

	bufferUpload->Release();
	return (true);
}

ID3D12Resource *D3D12Texture::getBuffer()
{
	return (_buffer);
}

D3D12_RESOURCE_DESC &D3D12Texture::getResourceDesc()
{
	return (_textureDesc);
}
