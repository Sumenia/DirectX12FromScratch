#include <iostream>
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12Device.h"
#include "MiniEngine/D3D12/D3D12CommandQueue.h"

using namespace MiniEngine;

D3D12RenderSystem::D3D12RenderSystem()
{}

D3D12RenderSystem::~D3D12RenderSystem()
{
    delete _commandQueue;
    delete _device;
    delete _factory;
}

bool D3D12RenderSystem::init()
{
    return (
        initDevice()
        && initCommandQueue()
    );
}

bool D3D12RenderSystem::initDevice()
{
    HRESULT                 result;

    result = CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&_factory);

    if (FAILED(result))
    {
        std::cout << "Failed to create DXGI Factory" << std::endl;
        return (false);
    }

    _device = D3D12Device::create();

    if (!_device)
    {
        std::cout << "Failed to create device" << std::endl;
        return (false);
    }

    return (true);
}

bool D3D12RenderSystem::initCommandQueue()
{
    _commandQueue = new D3D12CommandQueue(*this);
    return (_commandQueue->init());
}

IDXGIFactory4 *D3D12RenderSystem::getFactory()
{
    return (_factory);
}

D3D12Device *D3D12RenderSystem::getDevice()
{
    return (_device);
}

D3D12CommandQueue *D3D12RenderSystem::getCommandQueue()
{
    return (_commandQueue);
}