#pragma once

# include "MiniEngine/SceneManager.h"

namespace MiniEngine
{
    class BasicSceneManager : public SceneManager
    {
    public:
        BasicSceneManager(RenderSystem &system);
        ~BasicSceneManager();

        bool    render(Camera &camera, CommandList &commandList);
    };
}