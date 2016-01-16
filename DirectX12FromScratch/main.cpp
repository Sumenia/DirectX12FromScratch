
#include <iostream>
#include <d3d12.h>
#include <dxgi1_4.h>

//#include "IWindow.h"
#include "Window_SFML.h"
//#include <D3Dcompiler.h>
//#include <DirectXMath.h>

bool initD3D(ID3D12Device** device)
{
	HRESULT hr;

	// -- Create the Device -- //

	IDXGIFactory4* dxgiFactory;
	hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(hr))
	{
		return false;
	}

	IDXGIAdapter1* adapter; // adapters are the graphics card (this includes the embedded graphics on the motherboard)

	int adapterIndex = 0; // we'll start looking for directx 12  compatible graphics devices starting at index 0

	bool adapterFound = false; // set this to true when a good one was found

							   // find first hardware gpu that supports d3d 12
	while (dxgiFactory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// we dont want a software device
			continue;
		}

		// we want a device that is compatible with direct3d 12 (feature level 11 or higher)
		hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
		if (SUCCEEDED(hr))
		{
			adapterFound = true;
			break;
		}

		adapterIndex++;
	}

	if (!adapterFound)
	{
		return false;
	}

	// create the device
	hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(device));
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

int	main()
{
	//ID3D12Device* device;

	//if (initD3D(&device))
	//{
	//	std::cout << "SUCCESS" << std::endl;
	//	system("PAUSE");
	//	device->Release();
	//	std::cout << "device released." << std::endl;
	//	device = NULL;
	//}
	//else
	//	std::cout << "FAILURE" << std::endl;
	//system("PAUSE");

	IWindow *win = new Window_SFML();

	win->createWindow(800, 600);

	while (win->isOpen());

	delete (win);

	return (EXIT_SUCCESS);
}