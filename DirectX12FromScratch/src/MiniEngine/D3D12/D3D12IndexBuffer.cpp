#include "MiniEngine/Helper/d3dx12.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12Device.h"
#include "MiniEngine/D3D12/D3D12IndexBuffer.h"

using namespace MiniEngine;

D3D12IndexBuffer::D3D12IndexBuffer(D3D12RenderSystem &system) : _system(system), _buffer(nullptr), _bufferUpload(nullptr)
{}

D3D12IndexBuffer::~D3D12IndexBuffer()
{
    if (_buffer)
        _buffer->Release();

    _buffer = nullptr;

    if (_bufferUpload)
        _bufferUpload->Release();

    _bufferUpload = nullptr;
}

bool D3D12IndexBuffer::init(GraphicPipeline &pipeline, unsigned int size, void *indexData)
{
    HRESULT                 result;

    CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);

    CD3DX12_RESOURCE_DESC   indexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(size);
    D3D12_SUBRESOURCE_DATA  indexDataDesc = {};

    std::shared_ptr<D3D12CommandList>   commandList(_system.getCommandQueue()->createCommandList(nullptr, pipeline));

    if (!commandList->init())
        return (false);

    if (!commandList->reset())
        return (false);

    // Create buffer
    result = _system.getDevice()->getNative()->CreateCommittedResource(
        &defaultHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &indexBufferDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        __uuidof(ID3D12Resource),
        (void**)&_buffer
        );

    if (FAILED(result))
        return (false);

    // Create buffer upload
    result = _system.getDevice()->getNative()->CreateCommittedResource(
        &uploadHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &indexBufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        __uuidof(ID3D12Resource),
        (void**)&_bufferUpload
        );

    if (FAILED(result))
        return (false);

    // Upload vertex data
    indexDataDesc.pData = reinterpret_cast<BYTE*>(indexData);
    indexDataDesc.RowPitch = size;
    indexDataDesc.SlicePitch = indexDataDesc.RowPitch;

    UpdateSubresources(commandList->getNative(), _buffer, _bufferUpload, 0, 0, 1, &indexDataDesc);

    CD3DX12_RESOURCE_BARRIER indexBufferResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(_buffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);
    commandList->getNative()->ResourceBarrier(1, &indexBufferResourceBarrier);

    if (!commandList->end())
        return (false);

    _system.getCommandQueue()->executeCommandLists(1, commandList.get());
    return (_system.getCommandQueue()->wait(*_system.getFence()));
}

ID3D12Resource *D3D12IndexBuffer::getBuffer()
{
    return (_buffer);
}