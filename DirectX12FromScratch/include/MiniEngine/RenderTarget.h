#pragma once

# include "MiniEngine/RenderSystem.h"

namespace MiniEngine
{
    class RenderTarget
    {
    public:
        RenderTarget(RenderSystem &system);
        virtual ~RenderTarget();

    protected:
        RenderSystem        &_system;
    };
}