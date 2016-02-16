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
        virtual std::shared_ptr<Light>       createLight(Light::Type type, SceneNode *node = nullptr);
        virtual void        removeLight(Light *light);
        virtual void        updateLightBuffer();
        std::list<std::shared_ptr<Light>> const     getLights() const;

        virtual bool        render(Camera &camera, CommandList &commandList) = 0;

    protected:
        RenderSystem        &_system;
        SceneNode           *_rootNode;
        std::list<std::shared_ptr<Light>>   _lights;
        ConstantBuffer      *_lightsConstantBuffer;
        bool                _needUpdate;
    };
}