#pragma once

# include "MiniEngine/CommandList.h"
# include "MiniEngine/D3D12/D3D12GraphicPipeline.h"
# include "MiniEngine/D3D12/D3D12RenderTarget.h"

namespace MiniEngine
{
    class D3D12RenderSystem;

    class D3D12CommandList : public CommandList
    {
    public:
        D3D12CommandList(D3D12RenderSystem &system, D3D12RenderTarget *target);
        ~D3D12CommandList();

        bool                        init(bool bundle = false);
        bool                        reset();

        bool                        begin();
        bool                        end();

        bool                        bindCameraCBV(ConstantBuffer &buffer);
        bool                        bindModelCBV(ConstantBuffer &buffer);
        bool                        bindLightsCBV(ConstantBuffer &buffer);

        bool                        bindMaterial(Material &material);
        bool                        bindMaterial(unsigned int id);

        bool                        setPipeline(GraphicPipeline &pipeline);

        bool                        executeBundle(CommandList &list);

        ID3D12GraphicsCommandList   *getNative();

    protected:
        D3D12RenderSystem           &_system;
        ID3D12CommandAllocator      *_allocator;
        ID3D12GraphicsCommandList   *_list;

        ConstantBuffer              *_camera;
        ConstantBuffer              *_model;
        ConstantBuffer              *_lights;
    };
}
