#pragma once

# include "MiniEngine/RenderSystem.h"
# include "MiniEngine/GraphicPipeline.h"

namespace MiniEngine
{
    class RenderTarget
    {
    public:
        RenderTarget(RenderSystem &system);
        virtual ~RenderTarget();

		virtual bool		    init() = 0;

        virtual GraphicPipeline *getGraphicPipeline() = 0;

    protected:
        RenderSystem            &_system;
    };
}