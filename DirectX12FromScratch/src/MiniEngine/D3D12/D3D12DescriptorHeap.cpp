#include <iostream>
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12Device.h"
#include "MiniEngine/D3D12/D3D12DescriptorHeap.h"

using namespace MiniEngine;

D3D12DescriptorHeap::D3D12DescriptorHeap(D3D12RenderSystem &system) : _system(system)
{}

D3D12DescriptorHeap::~D3D12DescriptorHeap()
{}

bool D3D12DescriptorHeap::init(UINT nb, D3D12_DESCRIPTOR_HEAP_TYPE type)
{
	HRESULT						result;
	D3D12_DESCRIPTOR_HEAP_DESC	heapDesc = {};

	heapDesc.NumDescriptors = nb;
	heapDesc.Type = type;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	result = _system.getDevice()->getNative()->CreateDescriptorHeap(&heapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&_descriptorHeap);

	if (FAILED(result))
	{
		std::cout << "Can't create descriptor heap" << std::endl;
		return (false);
	}

	_size = _system.getDevice()->getNative()->GetDescriptorHandleIncrementSize(type);
}