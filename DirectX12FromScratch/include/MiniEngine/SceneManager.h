#pragma once

# include "MiniEngine/SceneNode.h"
# include "MiniEngine/Camera.h"
# include "MiniEngine/CommandList.h"

namespace MiniEngine
{
    class SceneManager
    {
    public:
        SceneManager();
        virtual ~SceneManager();

        virtual SceneNode   *getRootNode();

        virtual bool        render(Camera &camera, CommandList &commandList) = 0;

    protected:
        SceneNode           *_rootNode;
    };
}