#pragma once

# include "MiniEngine/RenderTarget.h"

namespace MiniEngine
{
	class D3D12RenderSystem;

	class D3D12RenderTarget: public RenderTarget
	{
	public:
		D3D12RenderTarget(D3D12RenderSystem &system);
		~D3D12RenderTarget();

	protected:
		D3D12RenderSystem        &_system;
	};
}