#pragma once

# include "MiniEngine/Shader.h"

namespace MiniEngine
{
    class RenderSystem;

    class GraphicPipeline
    {
    public:
        GraphicPipeline(RenderSystem &system);
        virtual ~GraphicPipeline();

        virtual void        setInputs(unsigned int nb, const void *inputs) = 0;

        virtual void        addVertexShader(Shader &shader) = 0;
        virtual void        addPixelShader(Shader &shader) = 0;

        virtual bool        finalize() = 0;

    protected:
        RenderSystem        &_system;
    };
}