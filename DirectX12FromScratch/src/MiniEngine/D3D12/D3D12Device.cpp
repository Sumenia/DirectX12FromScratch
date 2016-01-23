#include "MiniEngine/D3D12/D3D12Device.h"

using namespace MiniEngine;

D3D12Device::D3D12Device(ID3D12Device* device) : _device(device)
{}

D3D12Device::~D3D12Device()
{}

D3D12Device *D3D12Device::create(D3D_FEATURE_LEVEL featureLevel)
{
    ID3D12Device    *device;
    HRESULT         result;

    result = D3D12CreateDevice(NULL, featureLevel, __uuidof(ID3D12Device), (void**)&device);
    if (FAILED(result))
         return (nullptr);

    return (new D3D12Device(device));
}