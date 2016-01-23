#pragma once

# include "RenderSystem.h"
# include "D3D12Device.h"

namespace MiniEngine
{
    class D3D12RenderSystem : public RenderSystem
    {
    public:
        D3D12RenderSystem();
        ~D3D12RenderSystem();

        bool            init();

    protected:
        D3D12Device     *_device;
    };
}