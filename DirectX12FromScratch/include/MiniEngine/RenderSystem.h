#pragma once

# include <list>

namespace MiniEngine
{
    class RenderTarget;

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