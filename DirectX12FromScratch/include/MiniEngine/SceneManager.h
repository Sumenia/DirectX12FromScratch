#pragma once

# include <list>
# include "MiniEngine/SceneNode.h"
# include "MiniEngine/Camera.h"
# include "MiniEngine/CommandList.h"
# include "MiniEngine/Light.h"

namespace MiniEngine
{
    class RenderSystem;

    class SceneManager
    {
    public:
        SceneManager(RenderSystem &system);
        virtual ~SceneManager();

        virtual bool        init();

        virtual SceneNode   *getRootNode();

        virtual Camera      *createCamera(SceneNode *node = nullptr);
        virtual Light       *createLight(SceneNode *node = nullptr); // TO-DO: Take a type of light as parameter and create the good type
        virtual void        removeLight(Light *light);
        virtual void        updateLightBuffer();
        std::list<Light*> const     getLights() const;

        virtual bool        render(Camera &camera, CommandList &commandList) = 0;

    protected:
        RenderSystem        &_system;
        SceneNode           *_rootNode;
        std::list<Light*>   _lights;
        ConstantBuffer      *_lightsConstantBuffer;
        bool                _needUpdate;
    };
}