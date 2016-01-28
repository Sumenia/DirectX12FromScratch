#pragma once

# include <d3d12.h>
# include <dxgi1_4.h>
# include "Window.h"
# include "MiniEngine/RenderWindow.h"
# include "MiniEngine/D3D12/D3D12RenderTarget.h"
# include "MiniEngine/D3D12/D3D12DescriptorHeap.h"

namespace MiniEngine
{
    class D3D12RenderSystem;

    class D3D12RenderWindow : public RenderWindow, public D3D12RenderTarget
    {
    public:
        D3D12RenderWindow(D3D12RenderSystem &system, Window *window);
        ~D3D12RenderWindow();

		bool						init();

        bool                        initSwapChain();
		bool						initRtvDescriptorHeap();
		bool						initRtv();
        bool                        initCommandList();

        static const unsigned int   FrameCount = 2;

    protected:
        IDXGISwapChain3             *_swapChain;
		D3D12DescriptorHeap	        *_rtvDescriptorHeap;
		ID3D12Resource		        *_rtvs[D3D12RenderWindow::FrameCount];
        unsigned int                _frameIdx;
        D3D12CommandList            *_commandList;
    };
}