#pragma once

# include "MiniEngine/RenderSystem.h"

namespace MiniEngine
{
    class RenderTarget
    {
    public:
        RenderTarget(RenderSystem &system);
        virtual ~RenderTarget();

		virtual bool		init() = 0;

    protected:
        RenderSystem        &_system;
    };
}