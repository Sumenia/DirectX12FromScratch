#pragma once

# include "MiniEngine/RenderSystem.h"
# include "MiniEngine/D3D12/D3D12Device.h"

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