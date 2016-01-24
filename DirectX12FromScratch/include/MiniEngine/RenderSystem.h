#pragma once

# include <list>
# include "MiniEngine/CommandQueue.h"

namespace MiniEngine
{
    class RenderTarget;

    class RenderSystem
    {
    public:
        RenderSystem();
        virtual ~RenderSystem();

        virtual bool                init();
        virtual CommandQueue        *getCommandQueue() = 0;

        void                        addRenderTarget(RenderTarget *target);
        void                        clear();

    protected:
        std::list<RenderTarget*>    _targets;
    };
}