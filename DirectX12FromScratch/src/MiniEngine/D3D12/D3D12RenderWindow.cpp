#include <iostream>
#include "MiniEngine/D3D12/D3D12CommandQueue.h"
#include "MiniEngine/D3D12/D3D12RenderWindow.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12Device.h"
#include "MiniEngine/D3D12/D3D12ConstantBuffer.h"

using namespace MiniEngine;

D3D12RenderWindow::D3D12RenderWindow(D3D12RenderSystem &system, Window *window) : RenderTarget(system), RenderWindow(system, window), D3D12RenderTarget(system), _swapChain(nullptr), _commandList(nullptr)
{
	for (UINT n = 0; n < FrameCount; n++)
	{
		_rtvs[n] = nullptr;
	}
}

D3D12RenderWindow::~D3D12RenderWindow()
{
    delete _commandList;
    _commandList = nullptr;

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
        && initCommandList()
        && initConstantBuffers()
	);
}

bool D3D12RenderWindow::render()
{
    D3D12_RESOURCE_BARRIER      barrier;

    if (!_commandList->reset())
        return (false);

    // Set necessary state
    D3D12_RECT      scissorRect;

    _commandList->getNative()->SetGraphicsRootSignature(_system.getRootSignature()->getNative());

    // Bind constant buffer heaps
    D3D12DescriptorHeap *camerHeap = dynamic_cast<D3D12ConstantBuffer*>(_cameraConstantBuffer)->getHeap();

    ID3D12DescriptorHeap* ppHeaps[] = { camerHeap->getNative() };
    _commandList->getNative()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

    // Bind camera constant buffer to slot 0
    CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(camerHeap->getNative()->GetGPUDescriptorHandleForHeapStart(), _frameIdx, camerHeap->getSize());
    _commandList->getNative()->SetGraphicsRootDescriptorTable(0, gpuHandle);

    scissorRect.left = 0;
    scissorRect.top = 0;
    scissorRect.right = static_cast<LONG>(_window->getWidth());
    scissorRect.bottom = static_cast<LONG>(_window->getHeight());

    _commandList->getNative()->RSSetScissorRects(1, &scissorRect);

    // Set a ressource barrier
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = _rtvs[_frameIdx];
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

    _commandList->getNative()->ResourceBarrier(1, &barrier);

    // Set the current render target view
    CD3DX12_CPU_DESCRIPTOR_HANDLE renderTargetView(_rtvDescriptorHeap->getNative()->GetCPUDescriptorHandleForHeapStart(), _frameIdx, _rtvDescriptorHeap->getSize());
    _commandList->getNative()->OMSetRenderTargets(1, &renderTargetView, FALSE, nullptr);

    // Clear the render target view
    _commandList->getNative()->ClearRenderTargetView(renderTargetView, _clearColor, 0, nullptr);

    // Render all viewports
    for (auto &&viewport : _viewports)
    {
        // Set viewport
        D3D12_VIEWPORT  viewportRect;

        viewportRect.TopLeftX = static_cast<float>(viewport->getPosition().x);
        viewportRect.TopLeftY = static_cast<float>(viewport->getPosition().y);
        viewportRect.Width = static_cast<float>(viewport->getSize().x);
        viewportRect.Height = static_cast<float>(viewport->getSize().y);
        viewportRect.MaxDepth = D3D12_MAX_DEPTH;
        viewportRect.MinDepth = D3D12_MIN_DEPTH;

        _commandList->getNative()->RSSetViewports(1, &viewportRect);

        viewport->render(*_commandList);
    }

    // Reset viewport
    D3D12_VIEWPORT  viewportRect;

    viewportRect.TopLeftX = 0.0f;
    viewportRect.TopLeftY = 0.0f;
    viewportRect.Width = static_cast<float>(_window->getWidth());
    viewportRect.Height = static_cast<float>(_window->getHeight());
    viewportRect.MaxDepth = D3D12_MAX_DEPTH;
    viewportRect.MinDepth = D3D12_MIN_DEPTH;

    _commandList->getNative()->RSSetViewports(1, &viewportRect);

    // Indicate that the back buffer will now be used to present.
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

    _commandList->getNative()->ResourceBarrier(1, &barrier);

    // Close the list of commands.
    if (!_commandList->end())
        return (false);

    // Execute the list of commands.
    _system.getCommandQueue()->executeCommandLists(1, _commandList);

    if (!swap())
        return (false);

    return (waitPreviousFrame());
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

bool D3D12RenderWindow::initCommandList()
{
    _commandList = _system.getCommandQueue()->createCommandList(*this, *_pipeline);
    return (_commandList->init());
}

bool D3D12RenderWindow::initConstantBuffers()
{
    _cameraConstantBuffer = new D3D12ConstantBuffer(_system);
    return (_cameraConstantBuffer->init(128, D3D12RenderWindow::FrameCount));
}

bool D3D12RenderWindow::swap()
{
    HRESULT     result;

    result = _swapChain->Present(1, 0);

    if (FAILED(result))
    {
        std::cout << "Can't swap back buffer" << std::endl;
        return (false);
    }

    return (true);
}

bool D3D12RenderWindow::waitPreviousFrame()
{
    _frameIdx = (_frameIdx + 1) % D3D12RenderWindow::FrameCount;
    return (_system.getCommandQueue()->wait(*_system.getFence()));
}