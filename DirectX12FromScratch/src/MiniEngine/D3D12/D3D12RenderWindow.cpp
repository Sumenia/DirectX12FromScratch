#include <iostream>
#include "MiniEngine/D3D12/D3D12CommandQueue.h"
#include "MiniEngine/D3D12/D3D12RenderWindow.h"

using namespace MiniEngine;

D3D12RenderWindow::D3D12RenderWindow(D3D12RenderSystem &system, Window *window) : D3D12RenderTarget(system), _window(window), _swapChain(nullptr)
{}

D3D12RenderWindow::~D3D12RenderWindow()
{
    if (_swapChain)
        _swapChain->Release();
    _swapChain = nullptr;
}

bool D3D12RenderWindow::init()
{
	return (
		initSwapChain()
		&& initRtvDescriptorHeap()
	);
}

bool D3D12RenderWindow::initSwapChain()
{
    HRESULT                 result;
    DXGI_SWAP_CHAIN_DESC    swapChainDesc = {};

    swapChainDesc.BufferCount = D3D12RenderWindow::FrameCount;
    swapChainDesc.BufferDesc.Width = _window->getWidth();
    swapChainDesc.BufferDesc.Height = _window->getHeight();
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.OutputWindow = (HWND)_window->getHandle();
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = true;

    result = _system.getFactory()->CreateSwapChain(_system.getCommandQueue()->getNative(), &swapChainDesc, (IDXGISwapChain**)&_swapChain);

    if (FAILED(result))
    {
        std::cout << "Failed to create SwapChain" << std::endl;
        _swapChain = nullptr;

        return (false);
    }

    _frameIdx = _swapChain->GetCurrentBackBufferIndex();
	return (true);
}

bool D3D12RenderWindow::initRtvDescriptorHeap()
{
	_rtvDescriptorHeap = new D3D12DescriptorHeap(_system);
	return (_rtvDescriptorHeap->init(D3D12RenderWindow::FrameCount, D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
}