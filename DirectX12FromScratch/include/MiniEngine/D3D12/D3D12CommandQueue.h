#pragma once

# include <d3d12.h>
# include <dxgi1_4.h>
# include "MiniEngine/D3D12/D3D12RenderSystem.h"
# include "MiniEngine/CommandQueue.h"

namespace MiniEngine
{
    class D3D12CommandQueue : public CommandQueue
    {
    public:
        D3D12CommandQueue(D3D12RenderSystem &system);
        ~D3D12CommandQueue();

        bool                    init();
        ID3D12CommandQueue      *getNative();

    protected:
        D3D12RenderSystem       &_system;
        ID3D12CommandQueue      *_queue;
    };
}