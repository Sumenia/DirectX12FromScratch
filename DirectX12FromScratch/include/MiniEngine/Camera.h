#pragma once

# include "MiniEngine/Geometry.h"
# include "MiniEngine/MovableObject.h"
# include "MiniEngine/CommandList.h"

namespace MiniEngine
{
    class SceneManager;

    class Camera : public MovableObject
    {
    public:
        Camera(SceneManager &manager);
        ~Camera();

        bool            render(CommandList &commandList);

        void            lookAt(const Vector3f &eye, const Vector3f &target, const Vector3f &up);

        void            setFov(float fov);
        void            setRatio(float ratio);

        void            updateProjectionMatrix();
    
    protected:
        SceneManager    &_manager;

        float           _fov;
        float           _ratio;

        float           _near;
        float           _far;

        Matrix4f        _view;
        Matrix4f        _worldView;
        Matrix4f        _projection;
    };
}