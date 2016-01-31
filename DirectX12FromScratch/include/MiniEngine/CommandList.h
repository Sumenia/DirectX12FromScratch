#pragma once

# include "MiniEngine/GraphicPipeline.h"
# include "MiniEngine/Geometry.h"

namespace MiniEngine
{
    class RenderSystem;
    class RenderTarget;

    class CommandList
    {
    public:
        CommandList(RenderSystem &system, RenderTarget *target, GraphicPipeline &pipeline);
        virtual ~CommandList();

        virtual bool        init() = 0;
        virtual bool        reset() = 0;

        virtual bool        begin() = 0;
        virtual bool        end() = 0;

        virtual void        setCameraMatrix(Matrix4f const &view, Matrix4f const &projection) = 0;
        virtual void        afterCameraRender() = 0;

        RenderTarget        &getRenderTarget();

    protected:
        RenderSystem        &_system;
        RenderTarget        *_target;
        GraphicPipeline     &_pipeline;
    };
}