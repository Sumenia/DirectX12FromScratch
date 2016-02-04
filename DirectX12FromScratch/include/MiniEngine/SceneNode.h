#pragma once

# include "MiniEngine/Geometry.h"
# include <list>
# include <memory>
# include "MiniEngine/MovableObject.h"
# include "MiniEngine/CommandList.h"
# include "MiniEngine/RenderableObject.h"
# include "MiniEngine/ConstantBuffer.h"

namespace MiniEngine
{
    class SceneManager;
    class Camera;

    enum TransformSpace
    {
        TS_LOCAL,
        TS_PARENT,
        TS_WORLD
    };

    class SceneNode
    {
    public:
        SceneNode(SceneManager &manager, MovableObject *object = nullptr);
        virtual ~SceneNode();

        virtual SceneNode       *createChild(MovableObject *object = nullptr);
        virtual SceneNode       *addChild(SceneNode *node);
        virtual void            attachObject(MovableObject *obj);

        virtual SceneNode       *getParent();

        virtual bool            render(Camera &camera, CommandList &commandList);

        virtual Matrix4f        &getTransformationMatrix();

        Vector3f                &getDerivedPosition();
        Quatf                   &getDerivedRotation();
        Vector3f                &getDerivedScaling();

		virtual void			rotate(float w, Vector3f& v, TransformSpace space = TS_LOCAL);
		virtual void			translate(Vector3f& v, TransformSpace space = TS_LOCAL);
		virtual void			scale(Vector3f& v);

        virtual void            needUpdate();

    protected:
        virtual void            setParent(SceneNode *node);
        virtual void            update();

    protected:
        SceneManager            &_manager;

        SceneNode               *_parent;
        std::list<SceneNode*>   _childs;
        std::shared_ptr<MovableObject>  _obj;

        Vector3f                _position;
        Quatf                   _rotation;
        Vector3f                _scaling;

        Vector3f                _derivedPosition;
        Quatf                   _derivedRotation;
        Vector3f                _derivedScaling;

        Matrix4f                _transform;
        bool                    _needUpdate;

        ConstantBuffer          *_modelConstantBuffer;
    };
}