#pragma once

# include <list>
# include "RenderTarget.h"

namespace MiniEngine
{
    class RenderSystem
    {
    public:
        RenderSystem();
        virtual ~RenderSystem();

        virtual bool                init();

        void                        addRenderTarget(RenderTarget *target);

    protected:
        std::list<RenderTarget*>    _targets;
    };
}