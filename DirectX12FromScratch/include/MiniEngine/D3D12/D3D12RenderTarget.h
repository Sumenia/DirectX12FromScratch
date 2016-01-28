#pragma once

# include <d3d12.h>
# include "MiniEngine/RenderTarget.h"
# include "MiniEngine/D3D12/D3D12GraphicPipeline.h"

namespace MiniEngine
{
	class D3D12RenderSystem;

	class D3D12RenderTarget: public RenderTarget
	{
	public:
		D3D12RenderTarget(D3D12RenderSystem &system);
		~D3D12RenderTarget();

        virtual bool						    init() = 0;
        D3D12GraphicPipeline                    *getGraphicPipeline();

	protected:
		D3D12RenderSystem						&_system;
        D3D12GraphicPipeline                    *_pipeline;
	};
}