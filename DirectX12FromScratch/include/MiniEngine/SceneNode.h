#pragma once

# include <list>
# include "MiniEngine/MovableObject.h"
# include "MiniEngine/Camera.h"
# include "MiniEngine/CommandList.h"

namespace MiniEngine
{
    class SceneNode
    {
    public:
        SceneNode();
        ~SceneNode();

        virtual SceneNode       *createChild();
        virtual void            addChild(SceneNode *node);
        virtual void            attachObject(MovableObject *obj);

        virtual bool            render(Camera &camera, CommandList &commandList);

        // getTransformationMatrix
        // getWorldTransformationMatrix

    protected:
        virtual void            setParent(SceneNode *node);

    protected:
        SceneNode               *_parent;
        std::list<SceneNode*>   _childs;
        MovableObject           *_obj;

        // Position
        // Rotation
        // Scaling
    };
}