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
        virtual bool            render() = 0;

        void                    setClearColor(const float color[4]);

        virtual GraphicPipeline *getGraphicPipeline() = 0;

    protected:
        RenderSystem            &_system;
        float                   _clearColor[4];
    };
}