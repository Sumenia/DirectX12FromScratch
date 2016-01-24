#pragma once

# include <d3d12.h>
# include <dxgi1_4.h>
# include "MiniEngine/Fence.h"

namespace MiniEngine
{
    class D3D12RenderSystem;

    class D3D12Fence : public Fence
    {
    public:
        D3D12Fence(D3D12RenderSystem &system);
        ~D3D12Fence();

        bool                init();

        HANDLE              getEvent();
        ID3D12Fence         *getNative();
        UINT64              getValue();
        void                incValue();

    protected:
        D3D12RenderSystem   &_system;
        HANDLE              _fenceEvent;
        ID3D12Fence         *_fence;
        UINT64              _fenceValue;
    };
}