#pragma once

namespace MiniEngine
{
    class SceneNode;

    class MovableObject
    {
    public:
        MovableObject();
        virtual ~MovableObject();

        virtual void        setParent(SceneNode *node);
        virtual SceneNode   *getParent();

        virtual void        updateMatrix();

    protected:
        SceneNode       *_parent;
    };
}