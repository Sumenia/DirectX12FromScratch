#include <iostream>
#include "MiniEngine/D3D12/D3D12Fence.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12Device.h"

using namespace MiniEngine;

D3D12Fence::D3D12Fence(D3D12RenderSystem &system) : Fence(system), _system(system)
{}

D3D12Fence::~D3D12Fence()
{
    if (_fence)
        _fence->Release();
    _fence = nullptr;
}

bool D3D12Fence::init()
{
    HRESULT result;

	//		HRESULT result = static_cast<DeviceDX12 *>(inpDev)->dev->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&fence);
    result = _system.getDevice()->getNative()->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&_fence);

    if (FAILED(result))
    {
        std::cout << "Can't create fence" << std::endl;
        return (false);
    }

    _fenceValue = 1;
    _fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    if (_fenceEvent == nullptr)
    {
        std::cout << "Can't create an event" << std::endl;
        return (false);
    }

    return (true);
}

HANDLE D3D12Fence::getEvent()
{
    return (_fenceEvent);
}

ID3D12Fence *D3D12Fence::getNative()
{
    return (_fence);
}

UINT64 D3D12Fence::getValue()
{
    return (_fenceValue);
}

void D3D12Fence::incValue()
{
    _fenceValue++;
}