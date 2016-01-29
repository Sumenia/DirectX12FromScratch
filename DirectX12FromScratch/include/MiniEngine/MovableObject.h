#pragma once

namespace MiniEngine
{
    class MovableObject
    {
    public:
        virtual void    setParent(SceneNode *node);

    protected:
        SceneNode       *_parent;
    };
}