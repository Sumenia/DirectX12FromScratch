
#include "MiniEngine/D3D12/D3D12ConstantBuffer.h"
#include "MiniEngine/D3D12/D3D12Device.h"

using namespace MiniEngine;

D3D12ConstantBuffer::D3D12ConstantBuffer(D3D12RenderSystem &system) : ConstantBuffer(system), _system(system), _constantBufferUpload(nullptr), _constantBuffer(nullptr), _heap(nullptr)
{}

D3D12ConstantBuffer::~D3D12ConstantBuffer()
{
    if (_constantBuffer)
        _constantBuffer->Release();

    _constantBuffer = nullptr;

    if (_constantBufferUpload)
        _constantBufferUpload->Release();

    _constantBufferUpload = nullptr;

    delete _heap;
    _heap = nullptr;
}


bool D3D12ConstantBuffer::init(unsigned int size, unsigned int nb)
{
    size = (size + 255) & ~255;

    return (
        initHeap(nb)
        && initRessources(size, nb)
        && initView(size, nb)
    );
}

bool D3D12ConstantBuffer::initHeap(unsigned int nb)
{
    _heap = new D3D12DescriptorHeap(_system);
    return (_heap->init(nb, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE));
}

bool D3D12ConstantBuffer::initRessources(unsigned int size, unsigned int nb)
{
    CD3DX12_HEAP_PROPERTIES     defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_HEAP_PROPERTIES     uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);

    CD3DX12_RESOURCE_DESC       constantBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(size * nb);
    HRESULT                     result;

    result = _system.getDevice()->getNative()->CreateCommittedResource(
        &defaultHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &constantBufferDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        __uuidof(ID3D12Resource),
        (void**)&_constantBuffer
    );

    if (FAILED(result))
        return (false);

    result = _system.getDevice()->getNative()->CreateCommittedResource(
        &uploadHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &constantBufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        __uuidof(ID3D12Resource),
        (void**)&_constantBufferUpload
    );

    if (FAILED(result))
        return (false);

    return (true);
}

bool D3D12ConstantBuffer::initView(unsigned int size, unsigned int nb)
{
    D3D12_GPU_VIRTUAL_ADDRESS       cbvGpuAddress = _constantBuffer->GetGPUVirtualAddress();
    CD3DX12_CPU_DESCRIPTOR_HANDLE   cbvCpuHandle(_heap->getNative()->GetCPUDescriptorHandleForHeapStart());

    for (unsigned int n = 0; n < nb; n++)
    {
        D3D12_CONSTANT_BUFFER_VIEW_DESC desc;

        desc.BufferLocation = cbvGpuAddress;
        desc.SizeInBytes = size;

        _system.getDevice()->getNative()->CreateConstantBufferView(&desc, cbvCpuHandle);

        cbvGpuAddress += desc.SizeInBytes;
        cbvCpuHandle.Offset(_heap->getSize());
    }

    return (true);
}

void D3D12ConstantBuffer::update(CommandList &commandList, unsigned int size, void *data)
{
    D3D12_SUBRESOURCE_DATA descData = {};
    descData.pData = reinterpret_cast<BYTE*>(data);
    descData.RowPitch = size;
    descData.SlicePitch = descData.RowPitch;

    UpdateSubresources(dynamic_cast<D3D12CommandList&>(commandList).getNative(), _constantBuffer, _constantBufferUpload, 0, 0, 1, &descData);

    CD3DX12_RESOURCE_BARRIER bufferResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(_constantBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);
    dynamic_cast<D3D12CommandList&>(commandList).getNative()->ResourceBarrier(1, &bufferResourceBarrier);
}

void D3D12ConstantBuffer::afterUpdate(CommandList &commandList)
{
    CD3DX12_RESOURCE_BARRIER bufferResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(_constantBuffer, D3D12_RESOURCE_STATE_INDEX_BUFFER, D3D12_RESOURCE_STATE_COPY_DEST);
    dynamic_cast<D3D12CommandList&>(commandList).getNative()->ResourceBarrier(1, &bufferResourceBarrier);
}

D3D12DescriptorHeap *D3D12ConstantBuffer::getHeap()
{
    return (_heap);
}