#pragma once

# include "MiniEngine/RenderSystem.h"
# include "MiniEngine/SceneManager.h"

namespace MiniEngine
{
    class Root
    {
    public:
        enum SceneManagerType
        {
            BASIC
        };

    public:
        Root();
        ~Root();

        void            setRenderSystem(RenderSystem *renderSystem);
        RenderSystem    *getRenderSystem() const;

        SceneManager    *createSceneManager(SceneManagerType type);

    protected:
        RenderSystem    *_renderSystem;
    };
};