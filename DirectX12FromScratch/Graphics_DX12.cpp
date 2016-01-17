#include "Graphics_DX12.h"



Graphics_DX12::Graphics_DX12()
{
	m_device = nullptr;
	m_commandQueue = nullptr;
	m_swapChain = nullptr;
	m_renderTargetViewHeap = nullptr;
	m_backBufferRenderTarget[0] = nullptr;
	m_backBufferRenderTarget[1] = nullptr;
	m_commandAllocator = nullptr;
	m_commandList = nullptr;
	m_pipelineState = nullptr;
	m_fence = nullptr;
	m_fenceEvent = nullptr;
}


Graphics_DX12::~Graphics_DX12()
{
}

bool Graphics_DX12::Init(unsigned int width, unsigned int height, void* hwnd)
{
	D3D_FEATURE_LEVEL featureLevel;
	HRESULT result;
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
	
	IDXGIFactory4* factory;
	
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	
	unsigned int numModes, i, numerator, denominator, renderTargetViewDescriptorSize;
	unsigned long long stringLength;
	
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	
	int error;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	IDXGISwapChain* swapChain;

	D3D12_DESCRIPTOR_HEAP_DESC renderTargetViewHeapDesc;
	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle;

	m_vsync_enabled = false;

	featureLevel = D3D_FEATURE_LEVEL_11_0;
	result = D3D12CreateDevice(NULL, featureLevel, __uuidof(ID3D12Device), (void**)&m_device);
	if (FAILED(result))
	{
		MessageBox((HWND)hwnd, "Could not create a DirectX 12.1 device.  The default video card does not support DirectX 12.1.", "DirectX Device Failure", MB_OK);
		return false;
	}

	ZeroMemory(&commandQueueDesc, sizeof(commandQueueDesc));
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.NodeMask = 0;
	result = m_device->CreateCommandQueue(&commandQueueDesc, __uuidof(ID3D12CommandQueue), (void**)&m_commandQueue);
	if (FAILED(result))
	{
		return false;
	}

	result = CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	// On créé un adapteur pour la première carte graphique (la numéro 0) grâce à la factory.
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// On choisi la première sortie (la numéro 0) vidéo de cette carte.
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// Combien de mode d'affichages dispo ? (d'où le nullptr à la fin)
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	// On créé un tableau de la bonne taille.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// Même opération en mettant le resultat dans le tableau.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// On parcourt les modes pour trouver celui qui correspond à notre affichage/résolution.
	for (i = 0; i<numModes; i++)
	{
		if (displayModeList[i].Height == height)
		{
			if (displayModeList[i].Width == width)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}
	// On convertie la description de la carte en WCHAR vers CHAR.
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	// On libère les variables qui ne sont maintenant plus nécessaires.
	delete[] displayModeList;
	displayModeList = nullptr;
	adapterOutput->Release();
	adapterOutput = nullptr;
	adapter->Release();
	adapter = nullptr;

	// SWAP CHAIN

	// On configure la swap chain avant de la creer.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	// C'est ici qu'on spécifie la fenêtre à utiliser
	swapChainDesc.OutputWindow = (HWND)hwnd;
	// V-sync : cette config ne la prend pas en compte.
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// OMG la config de fou! reste plus qu'à creer la swapchain à l'aide de la factory...
	result = factory->CreateSwapChain(m_commandQueue, &swapChainDesc, &swapChain);
	if (FAILED(result))
	{
		return false;
	}
	// génial en faite on converti cette swapchain vers la conversion 3 (que l'on garde dans m_swapChain).
	result = swapChain->QueryInterface(__uuidof(IDXGISwapChain3), (void**)&m_swapChain);
	if (FAILED(result))
	{
		return false;
	}
	swapChain = nullptr;

	// La factory n'est plus nécessaire.
	factory->Release();
	factory = nullptr;

	// On configure notre device pour qu'elle utilise cette swapChain...
	ZeroMemory(&renderTargetViewHeapDesc, sizeof(renderTargetViewHeapDesc));
	renderTargetViewHeapDesc.NumDescriptors = 2;
	renderTargetViewHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	renderTargetViewHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	// Create the render target view heap for the back buffers.
	result = m_device->CreateDescriptorHeap(&renderTargetViewHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_renderTargetViewHeap);
	if (FAILED(result))
	{
		return false;
	}

	// Get a handle to the starting memory location in the render target view heap to identify where the render target views will be located for the two back buffers.
	renderTargetViewHandle = m_renderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();

	// Get the size of the memory location for the render target view descriptors.
	renderTargetViewDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// Get a pointer to the first back buffer from the swap chain.
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D12Resource), (void**)&m_backBufferRenderTarget[0]);
	if (FAILED(result))
	{
		return false;
	}

	// Create a render target view for the first back buffer.
	m_device->CreateRenderTargetView(m_backBufferRenderTarget[0], nullptr, renderTargetViewHandle);

	// Increment the view handle to the next descriptor location in the render target view heap.
	renderTargetViewHandle.ptr += renderTargetViewDescriptorSize;

	// Get a pointer to the second back buffer from the swap chain.
	result = m_swapChain->GetBuffer(1, __uuidof(ID3D12Resource), (void**)&m_backBufferRenderTarget[1]);
	if (FAILED(result))
	{
		return false;
	}

	// Create a render target view for the second back buffer.
	m_device->CreateRenderTargetView(m_backBufferRenderTarget[1], nullptr, renderTargetViewHandle);

	m_bufferIndex = m_swapChain->GetCurrentBackBufferIndex();

	// Command allocator
	result = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&m_commandAllocator);
	if (FAILED(result))
	{
		return false;
	}

	// Create a basic command list.
	result = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator, nullptr, __uuidof(ID3D12GraphicsCommandList), (void**)&m_commandList);
	if (FAILED(result))
	{
		return false;
	}

	// Initially we need to close the command list during initialization as it is created in a recording state.
	result = m_commandList->Close();
	if (FAILED(result))
	{
		return false;
	}

	// FENCE !!!
	result = m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&m_fence);
	if (FAILED(result))
	{
		return false;
	}

	// Create an event object for the fence.
	m_fenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
	if (m_fenceEvent == nullptr)
	{
		return false;
	}

	// Initialize the starting fence value. 
	m_fenceValue = 1;

	return true;
}

bool Graphics_DX12::setOutputDisplay(void *)
{
	return false;
}

bool Graphics_DX12::Display()
{
	HRESULT result;
	D3D12_RESOURCE_BARRIER barrier;
	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle;
	unsigned int renderTargetViewDescriptorSize;
	float color[4];
	ID3D12CommandList* ppCommandLists[1];
	unsigned long long fenceToWaitFor;

	// Reset (re-use) the memory associated command allocator.
	result = m_commandAllocator->Reset();
	if (FAILED(result))
	{
		return false;
	}

	// Reset the command list, use empty pipeline state for now since there are no shaders and we are just clearing the screen.
	result = m_commandList->Reset(m_commandAllocator, m_pipelineState);
	if (FAILED(result))
	{
		return false;
	}

	// Record commands in the command list now.
	// Start by setting the resource barrier.
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_backBufferRenderTarget[m_bufferIndex];
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	m_commandList->ResourceBarrier(1, &barrier);

	// Get the render target view handle for the current back buffer.
	renderTargetViewHandle = m_renderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();
	renderTargetViewDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	if (m_bufferIndex == 1)
	{
		renderTargetViewHandle.ptr += renderTargetViewDescriptorSize;
	}

	// Set the back buffer as the render target.
	m_commandList->OMSetRenderTargets(1, &renderTargetViewHandle, FALSE, nullptr);

	// Then set the color to clear the window to.
	color[0] = 0.5;
	color[1] = 0.5;
	color[2] = 0.5;
	color[3] = 1.0;
	m_commandList->ClearRenderTargetView(renderTargetViewHandle, color, 0, nullptr);

	// Indicate that the back buffer will now be used to present.
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	m_commandList->ResourceBarrier(1, &barrier);

	// Close the list of commands.
	result = m_commandList->Close();
	if (FAILED(result))
	{
		return false;
	}

	// Load the command list array (only one command list for now).
	ppCommandLists[0] = m_commandList;

	// Execute the list of commands.
	m_commandQueue->ExecuteCommandLists(1, ppCommandLists);

	// Finally present the back buffer to the screen since rendering is complete.
	if (m_vsync_enabled)
	{
		// Lock to screen refresh rate.
		result = m_swapChain->Present(1, 0);
		if (FAILED(result))
		{
			return false;
		}
	}
	else
	{
		// Present as fast as possible.
		result = m_swapChain->Present(0, 0);
		if (FAILED(result))
		{
			return false;
		}
	}

	// Signal and increment the fence value.
	fenceToWaitFor = m_fenceValue;
	result = m_commandQueue->Signal(m_fence, fenceToWaitFor);
	if (FAILED(result))
	{
		return false;
	}
	m_fenceValue++;

	// Wait until the GPU is done rendering.
	if (m_fence->GetCompletedValue() < fenceToWaitFor)
	{
		result = m_fence->SetEventOnCompletion(fenceToWaitFor, m_fenceEvent);
		if (FAILED(result))
		{
			return false;
		}
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}

	// Alternate the back buffer index back and forth between 0 and 1 each frame.
	m_bufferIndex == 0 ? m_bufferIndex = 1 : m_bufferIndex = 0;

	return true;
}

void Graphics_DX12::Quit()
{
	int error;


	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, nullptr);
	}

	error = CloseHandle(m_fenceEvent);
	if (error == 0)
	{
	}

	// Release the fence.
	if (m_fence)
	{
		m_fence->Release();
		m_fence = 0;
	}

	// Release the empty pipe line state.
	if (m_pipelineState)
	{
		m_pipelineState->Release();
		m_pipelineState = 0;
	}

	// Release the command list.
	if (m_commandList)
	{
		m_commandList->Release();
		m_commandList = 0;
	}

	// Release the command allocator.
	if (m_commandAllocator)
	{
		m_commandAllocator->Release();
		m_commandAllocator = 0;
	}

	// Release the back buffer render target views.
	if (m_backBufferRenderTarget[0])
	{
		m_backBufferRenderTarget[0]->Release();
		m_backBufferRenderTarget[0] = 0;
	}
	if (m_backBufferRenderTarget[1])
	{
		m_backBufferRenderTarget[1]->Release();
		m_backBufferRenderTarget[1] = 0;
	}

	// Release the render target view heap.
	if (m_renderTargetViewHeap)
	{
		m_renderTargetViewHeap->Release();
		m_renderTargetViewHeap = 0;
	}

	// Release the swap chain.
	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}

	// Release the command queue.
	if (m_commandQueue)
	{
		m_commandQueue->Release();
		m_commandQueue = 0;
	}

	// Release the device.
	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}
}
