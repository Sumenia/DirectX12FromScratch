#include "MiniEngine/D3D12/D3D12Device.h"

using namespace MiniEngine;

D3D12Device::D3D12Device(ID3D12Device* device) : _device(device)
{}

D3D12Device::~D3D12Device()
{
    if (_device)
        _device->Release();
    _device = nullptr;
}

ID3D12Device *D3D12Device::getNative()
{
    return (_device);
}

D3D12Device *D3D12Device::create(IDXGIFactory4* factory, D3D_FEATURE_LEVEL featureLevel)
{
    ID3D12Device    *device;
	IDXGIAdapter1	*adapter = nullptr;
	int				adapterIdx;
	bool			found;

	adapterIdx = 0;
	found = false;

	while (!found && factory->EnumAdapters1(adapterIdx, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (!(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) &&
			SUCCEEDED(D3D12CreateDevice(adapter, featureLevel, __uuidof(ID3D12Device), nullptr)))
			found = true;
		adapterIdx++;
	}

	if (found &&
		SUCCEEDED(D3D12CreateDevice(adapter, featureLevel, __uuidof(ID3D12Device), (void**)&device)))
		return (new D3D12Device(device));

	return (nullptr);
}