#pragma once

#include "IGraphics.h"

#include <d3d12.h>
#include <dxgi1_4.h>

class Graphics_DX12: public IGraphics
{
public:
	Graphics_DX12();
	~Graphics_DX12();

	// Hérité via IGraphics
	virtual bool Init(unsigned int width, unsigned int height, void* hwnd);
	virtual bool setOutputDisplay(void *);
	virtual void Quit();

	virtual bool Display();

private:
	bool m_vsync_enabled;
	ID3D12Device* m_device;
	ID3D12CommandQueue* m_commandQueue;
	char m_videoCardDescription[128];
	IDXGISwapChain3* m_swapChain;
	ID3D12DescriptorHeap* m_renderTargetViewHeap;
	ID3D12Resource* m_backBufferRenderTarget[2];
	unsigned int m_bufferIndex;
	ID3D12CommandAllocator* m_commandAllocator;
	ID3D12GraphicsCommandList* m_commandList;
	ID3D12PipelineState* m_pipelineState;
	ID3D12Fence* m_fence;
	HANDLE m_fenceEvent;
	unsigned long long m_fenceValue;
};

