#pragma once

# include "RenderSystem.h"

namespace MiniEngine
{
    class Root
    {
    public:
        Root();
        ~Root();

        void            setRenderSystem(RenderSystem *renderSystem);
        RenderSystem    *getRenderSystem() const;

    protected:
        RenderSystem    *_renderSystem;
    };
};