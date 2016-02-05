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

        virtual void        needUpdate();

    protected:
        SceneNode           *_parent;
        bool                _needUpdate;
    };
}