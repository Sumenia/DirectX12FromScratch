#include <iostream>
#include "MiniEngine/D3D12/D3D12CommandQueue.h"
#include "MiniEngine/D3D12/D3D12RenderWindow.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12Device.h"

using namespace MiniEngine;

D3D12RenderWindow::D3D12RenderWindow(D3D12RenderSystem &system, Window *window) : D3D12RenderTarget(system), _window(window), _swapChain(nullptr)
{
	for (UINT n = 0; n < FrameCount; n++)
	{
		_rtvs[n] = nullptr;
	}
}

D3D12RenderWindow::~D3D12RenderWindow()
{
	for (UINT n = 0; n < FrameCount; n++)
		_rtvs[n]->Release();

	delete _rtvDescriptorHeap;
	_rtvDescriptorHeap = nullptr;

    if (_swapChain)
        _swapChain->Release();

    _swapChain = nullptr;
}

bool D3D12RenderWindow::init()
{
	return (
		initSwapChain()
		&& initRtvDescriptorHeap()
		&& initRtv()
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

bool D3D12RenderWindow::initRtv()
{
	HRESULT							result;
	CD3DX12_CPU_DESCRIPTOR_HANDLE	rtvHandle(_rtvDescriptorHeap->getNative()->GetCPUDescriptorHandleForHeapStart());

	for (UINT n = 0; n < FrameCount; n++)
	{
		result = _swapChain->GetBuffer(n, __uuidof(ID3D12Resource), (void**)&_rtvs[n]);
		
		if (FAILED(result))
		{
			std::cout << "Can't get buffer" << std::endl;
			return (false);
		}

		_system.getDevice()->getNative()->CreateRenderTargetView(_rtvs[n], nullptr, rtvHandle);

		rtvHandle.Offset(1, _rtvDescriptorHeap->getSize());
	}

	return (true);
}