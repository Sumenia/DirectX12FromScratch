#pragma once

# include <d3d12.h>
# include <dxgi1_4.h>
# include "Window.h"
# include "MiniEngine/RenderTarget.h"
# include "MiniEngine/D3D12/D3D12RenderSystem.h"

namespace MiniEngine
{
    class D3D12RenderWindow : public RenderTarget
    {
    public:
        D3D12RenderWindow(D3D12RenderSystem &system, Window *window);
        ~D3D12RenderWindow();

        void                        initSwapChain(Window *window);

        static const unsigned int   FrameCount = 2;

    protected:
        D3D12RenderSystem   &_system;
        IDXGISwapChain3     *_swapChain;
        unsigned int        _frameIdx;
    };
}