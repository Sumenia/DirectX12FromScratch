#pragma once

# include <d3d12.h>
# include <dxgi1_4.h>
# include "MiniEngine/Helper/d3dx12.h"

namespace MiniEngine
{
    class D3D12RenderSystem;

    class D3D12RootSignature
    {
    public:
        D3D12RootSignature();
        ~D3D12RootSignature();

        bool                    init(D3D12RenderSystem &system, bool ambient, bool diffuse, bool specular);

        ID3D12RootSignature     *getNative();

    protected:
        ID3D12RootSignature     *_signature;
    };
}