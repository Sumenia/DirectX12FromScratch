#pragma once

# include <d3d12.h>
# include <dxgi1_4.h>

namespace MiniEngine
{
	class D3D12RenderSystem;

	class D3D12DescriptorHeap
	{
	public:
		D3D12DescriptorHeap(D3D12RenderSystem &system);
		~D3D12DescriptorHeap();

		bool					init(UINT nb, D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

		ID3D12DescriptorHeap	*getNative();
		UINT					getSize() const;

	protected:
		D3D12RenderSystem		&_system;
		ID3D12DescriptorHeap	*_descriptorHeap;
		UINT					_size;
	};
}