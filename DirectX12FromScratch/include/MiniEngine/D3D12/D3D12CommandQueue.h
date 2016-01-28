#pragma once

# include <d3d12.h>
# include <dxgi1_4.h>
# include "MiniEngine/CommandQueue.h"
# include "MiniEngine/D3D12/D3D12Fence.h"
# include "MiniEngine/D3D12/D3D12CommandList.h"

namespace MiniEngine
{
    class D3D12RenderSystem;

    class D3D12CommandQueue : public CommandQueue
    {
    public:
        D3D12CommandQueue(D3D12RenderSystem &system);
        ~D3D12CommandQueue();

        D3D12CommandList        *createCommandList(GraphicPipeline &pipeline);

        bool                    wait(Fence &fence);
        bool                    wait(D3D12Fence &fence);

        bool                    executeCommandLists(unsigned int nb, CommandList *list);

        bool                    init();
        ID3D12CommandQueue      *getNative();

    protected:
        D3D12RenderSystem       &_system;
        ID3D12CommandQueue      *_queue;
    };
}