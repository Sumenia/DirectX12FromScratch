#pragma once

# include "MiniEngine/SceneNode.h"
# include "MiniEngine/Geometry.h"
# include "MiniEngine/CommandList.h"

namespace MiniEngine
{
    class SceneManager;

    class Camera : public SceneNode
    {
    public:
        Camera(SceneManager &manager);
        ~Camera();

        bool            render(CommandList &commandList);

        void            lookAt(const Vector3f &eye, const Vector3f &target, const Vector3f &up);

        void            setFov(float fov);
        void            setRatio(float ratio);

        void            updateProjectionMatrix();

		const Vector3f	&getPos() const;

    protected:
        float           _fov;
        float           _ratio;

        float           _near;
        float           _far;

		Vector3f		_pos;

        Matrix4f        _view;
        Matrix4f        _worldView;
        Matrix4f        _projection;

        ConstantBuffer  *_cameraConstantBuffer;
    };
}