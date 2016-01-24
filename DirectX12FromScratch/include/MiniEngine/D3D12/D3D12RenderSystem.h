#pragma once

# include <dxgi1_4.h>
# include "MiniEngine/RenderSystem.h"
# include "MiniEngine/D3D12/D3D12CommandQueue.h"

namespace MiniEngine
{
    class D3D12Device;

    class D3D12RenderSystem : public RenderSystem
    {
    public:
        D3D12RenderSystem();
        ~D3D12RenderSystem();

        bool                init();
        bool                initDevice();
        bool                initCommandQueue();

        IDXGIFactory4       *getFactory();
        D3D12Device         *getDevice();
        D3D12CommandQueue   *getCommandQueue();

    protected:
        IDXGIFactory4       *_factory;
        D3D12Device         *_device;
        D3D12CommandQueue   *_commandQueue;
    };
}