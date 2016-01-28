#pragma once

# include "MiniEngine/CommandList.h"
# include "MiniEngine/D3D12/D3D12GraphicPipeline.h"

namespace MiniEngine
{
    class D3D12RenderSystem;

    class D3D12CommandList : public CommandList
    {
    public:
        D3D12CommandList(D3D12RenderSystem &system, D3D12GraphicPipeline &pipeline);
        ~D3D12CommandList();

        bool                        init();
        bool                        reset();

        bool                        begin();
        bool                        end();

    protected:
        D3D12RenderSystem           &_system;
        D3D12GraphicPipeline        &_pipeline;
        ID3D12CommandAllocator      *_allocator;
        ID3D12GraphicsCommandList   *_list;
    };
}
