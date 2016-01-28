#pragma once

# include "MiniEngine/GraphicPipeline.h"

namespace MiniEngine
{
    class RenderSystem;

    class CommandList
    {
    public:
        CommandList(RenderSystem &system, GraphicPipeline &pipeline);
        virtual ~CommandList();

        virtual bool        init() = 0;
        virtual bool        reset() = 0;

        virtual bool        begin() = 0;
        virtual bool        end() = 0;

    protected:
        RenderSystem        &_system;
        GraphicPipeline     &_pipeline;
    };
}