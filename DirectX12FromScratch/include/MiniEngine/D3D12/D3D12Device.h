#pragma once

#include <d3d12.h>

namespace MiniEngine
{
    class D3D12Device
    {
    public:
        D3D12Device(ID3D12Device* device);
        ~D3D12Device();

        ID3D12Device            *getNative();

        static D3D12Device      *create(D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0);

    protected:
        ID3D12Device            *_device;
    };
}