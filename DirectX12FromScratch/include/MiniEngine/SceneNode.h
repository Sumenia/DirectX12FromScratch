#pragma once

# include "MiniEngine/Geometry.h"
# include <list>
# include <memory>
# include "MiniEngine/MovableObject.h"
# include "MiniEngine/Camera.h"
# include "MiniEngine/CommandList.h"
# include "MiniEngine/RenderableObject.h"

namespace MiniEngine
{
    class SceneManager;

    class SceneNode
    {
    public:
        SceneNode(SceneManager &manager, MovableObject *object = nullptr);
        ~SceneNode();

        virtual SceneNode       *createChild(MovableObject *object = nullptr);
        virtual SceneNode       *addChild(SceneNode *node);
        virtual void            attachObject(MovableObject *obj);

        virtual SceneNode       *getParent();

        virtual bool            render(Camera &camera, CommandList &commandList);

        virtual Matrix4f const &getTransformationMatrix() const;
        virtual Matrix4f const &getWorldTransformationMatrix() const;

		virtual void			rotate(float w, Vector3f& v);
		virtual void			translate(Vector3f& v);
		virtual void			scale(Vector3f& v);

        virtual void            updateMatrix();

    protected:
        virtual void            setParent(SceneNode *node);

    protected:
        SceneManager            &_manager;

        SceneNode               *_parent;
        std::list<SceneNode*>   _childs;
        std::shared_ptr<MovableObject>  _obj;

        Vector3f                _position;
        Quatf                   _rotation;
        Vector3f                _scaling;

        Matrix4f                _localMatrix;
        Matrix4f                _worldMatrix;
    };
}