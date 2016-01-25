#pragma once

# include <d3d12.h>
# include "MiniEngine/RenderTarget.h"

namespace MiniEngine
{
	class D3D12RenderSystem;

	class D3D12RenderTarget: public RenderTarget
	{
	public:
		D3D12RenderTarget(D3D12RenderSystem &system);
		~D3D12RenderTarget();

		bool	init();

	protected:
		D3D12RenderSystem						&_system;
		D3D12_DESCRIPTOR_HEAP_DESC				_rtvHeapDesc;
		void*									_rtvHeap;
	};
}