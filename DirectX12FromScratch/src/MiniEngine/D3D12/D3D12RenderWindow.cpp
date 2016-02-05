#include <iostream>
#include "MiniEngine/D3D12/D3D12CommandQueue.h"
#include "MiniEngine/D3D12/D3D12RenderWindow.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12Device.h"
#include "MiniEngine/D3D12/D3D12ConstantBuffer.h"

using namespace MiniEngine;

D3D12RenderWindow::D3D12RenderWindow(D3D12RenderSystem &system, Window *window) : RenderTarget(system), RenderWindow(system, window), D3D12RenderTarget(system), _swapChain(nullptr), _commandList(nullptr), _rtvDescriptorHeap(nullptr), _dsvDescriptorHeap(nullptr)
{
	for (UINT n = 0; n < FrameCount; n++)
	{
		_rtvs[n] = nullptr;
	}

    _frameCount = FrameCount;
}

D3D12RenderWindow::~D3D12RenderWindow()
{
    delete _commandList;
    _commandList = nullptr;

    if (_dsv)
        _dsv->Release();

    _dsv = nullptr;

    delete _dsvDescriptorHeap;
    _dsvDescriptorHeap = nullptr;

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
        && initDsvDescriptorHeap()
        && initDsv()
        && initCommandList()
	);
}

bool D3D12RenderWindow::render()
{
    if (!_commandList->reset())
        return (false);

    // Set necessary state
    D3D12_RECT      scissorRect;

    scissorRect.left = 0;
    scissorRect.top = 0;
    scissorRect.right = static_cast<LONG>(_window->getWidth());
    scissorRect.bottom = static_cast<LONG>(_window->getHeight());

    _commandList->getNative()->RSSetScissorRects(1, &scissorRect);

    // Set a ressource barrier
	CD3DX12_RESOURCE_BARRIER renderTargetResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(_rtvs[_frameIdx], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    _commandList->getNative()->ResourceBarrier(1, &renderTargetResourceBarrier);

    CD3DX12_CPU_DESCRIPTOR_HANDLE renderTargetView(_rtvDescriptorHeap->getNative()->GetCPUDescriptorHandleForHeapStart(), _frameIdx, _rtvDescriptorHeap->getSize());
	CD3DX12_CPU_DESCRIPTOR_HANDLE depthStencilView(_dsvDescriptorHeap->getNative()->GetCPUDescriptorHandleForHeapStart());

	// Clear the render target view
	_commandList->getNative()->ClearRenderTargetView(renderTargetView, _clearColor, 0, nullptr);

	// Clear the depth stencil
	_commandList->getNative()->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	
	// Set the current render target view and depth stencil view
	_commandList->getNative()->OMSetRenderTargets(1, &renderTargetView, FALSE, &depthStencilView);

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

        if (!viewport->render(*_commandList))
            return (false);
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
	CD3DX12_RESOURCE_BARRIER presentResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(_rtvs[_frameIdx], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	_commandList->getNative()->ResourceBarrier(1, &presentResourceBarrier);

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
    DXGI_SWAP_CHAIN_DESC1   swapChainDesc = {};

	swapChainDesc.Width = _window->getWidth();
	swapChainDesc.Height = _window->getHeight();
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = D3D12RenderWindow::FrameCount;
	swapChainDesc.Scaling = DXGI_SCALING_NONE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags = 0;

    result = _system.getFactory()->CreateSwapChainForHwnd(_system.getCommandQueue()->getNative(), (HWND)_window->getHandle(), &swapChainDesc, NULL, NULL, (IDXGISwapChain1**)&_swapChain);

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

bool D3D12RenderWindow::initDsvDescriptorHeap()
{
    _dsvDescriptorHeap = new D3D12DescriptorHeap(_system);
    return (_dsvDescriptorHeap->init(1, D3D12_DESCRIPTOR_HEAP_TYPE_DSV));
}

bool D3D12RenderWindow::initDsv()
{
    HRESULT                         result;

    D3D12_CLEAR_VALUE               depthOptimizedClearValue = {};
    D3D12_DEPTH_STENCIL_VIEW_DESC   dsvDesc = {};

    D3D12_HEAP_PROPERTIES           depthHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    D3D12_RESOURCE_DESC             depthResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(
        DXGI_FORMAT_D32_FLOAT,
        static_cast<UINT>(_window->getWidth()),
        static_cast<UINT>(_window->getHeight()),
        1,
        0,
        1,
        0,
        D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
    );

    depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
    depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
    depthOptimizedClearValue.DepthStencil.Stencil = 0;

    result = _system.getDevice()->getNative()->CreateCommittedResource(
        &depthHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &depthResourceDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &depthOptimizedClearValue,
        __uuidof(ID3D12Resource), (void**)&_dsv
    );

    if (FAILED(result))
    {
        std::cout << "Can't create ressource for DSV" << std::endl;
        return (false);
    }

    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

    _system.getDevice()->getNative()->CreateDepthStencilView(_dsv, &dsvDesc, _dsvDescriptorHeap->getNative()->GetCPUDescriptorHandleForHeapStart());
    return (true);
}

bool D3D12RenderWindow::initCommandList()
{
    _commandList = _system.getCommandQueue()->createCommandList(this);
    return (_commandList->init());
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