#include <iostream>
#include "MiniEngine/D3D12/D3D12CommandQueue.h"
#include "MiniEngine/D3D12/D3D12RenderWindow.h"

using namespace MiniEngine;

D3D12RenderWindow::D3D12RenderWindow(D3D12RenderSystem &system, Window *window) : RenderTarget(system), _system(system), _swapChain(nullptr)
{
    initSwapChain(window);
}

D3D12RenderWindow::~D3D12RenderWindow()
{
    if (_swapChain)
        _swapChain->Release();
    _swapChain = nullptr;
}

void D3D12RenderWindow::initSwapChain(Window *window)
{
    HRESULT                 result;
    DXGI_SWAP_CHAIN_DESC    swapChainDesc = {};

    swapChainDesc.BufferCount = D3D12RenderWindow::FrameCount;
    swapChainDesc.BufferDesc.Width = window->getWidth();
    swapChainDesc.BufferDesc.Height = window->getHeight();
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.OutputWindow = (HWND)window->getHandle();
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = true;

    result = _system.getFactory()->CreateSwapChain(_system.getCommandQueue()->getNative(), &swapChainDesc, (IDXGISwapChain**)&_swapChain);

    if (FAILED(result))
    {
        std::cout << "Failed to create SwapChain" << std::endl;
        _swapChain = nullptr;

        return;
    }

    _frameIdx = _swapChain->GetCurrentBackBufferIndex();
}