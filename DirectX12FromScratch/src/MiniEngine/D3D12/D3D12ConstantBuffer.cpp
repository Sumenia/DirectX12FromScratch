#include "MiniEngine/D3D12/D3D12ConstantBuffer.h"
#include "MiniEngine/D3D12/D3D12Device.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"

using namespace MiniEngine;

D3D12ConstantBuffer::D3D12ConstantBuffer(D3D12RenderSystem &system) : ConstantBuffer(system), _system(system), _constantBuffer(nullptr), _nb(0), _size(0)
{}

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

bool D3D12ConstantBuffer::update(CommandList &commandList, unsigned int rootIdx, unsigned int size, void *data)
{
    unsigned int idx = commandList.getRenderTarget().getFrameIdx();

    CD3DX12_RANGE   readRange(0, 0);
    UINT8           *mappedMemory = nullptr;
    HRESULT         result;

    result = _constantBuffer[idx]->Map(0, &readRange, reinterpret_cast<void**>(&mappedMemory));

    if (FAILED(result))
        return (false);

    memcpy(mappedMemory, data, size);

    _constantBuffer[idx]->Unmap(0, &readRange);

    dynamic_cast<D3D12CommandList&>(commandList).getNative()->SetGraphicsRootConstantBufferView(rootIdx, _constantBuffer[idx]->GetGPUVirtualAddress());
    return (true);
}

ID3D12Resource *D3D12ConstantBuffer::getNative(unsigned int idx)
{
    return (_constantBuffer[idx]);
}