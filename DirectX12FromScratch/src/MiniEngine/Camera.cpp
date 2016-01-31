#include "MiniEngine/Camera.h"
#include "MiniEngine/SceneManager.h"

using namespace MiniEngine;

Camera::Camera(SceneManager &manager) : _manager(manager), _fov(90.0f), _ratio(16.0f / 9.0f), _near(0.1f), _far(100.0f)
{}

Camera::~Camera()
{}

bool Camera::render(CommandList &commandList)
{
    commandList.setCameraMatrix(_view, _projection);
    return  (_manager.render(*this, commandList));
}

void Camera::updateMatrix()
{
    if (_parent)
        _worldView = _view * _parent->getWorldTransformationMatrix();
    else
        _worldView = _view;
}

void Camera::lookAt(const Vector3f &eye, const Vector3f &target, const Vector3f &up)
{
    _view = Matrix4f::createLookAt(eye, target, up);
    updateMatrix();
}

void Camera::setFov(float fov)
{
    _fov = fov;
    updateProjectionMatrix();
}

void Camera::setRatio(float ratio)
{
    _ratio = ratio;
    updateProjectionMatrix();
}

void Camera::updateProjectionMatrix()
{
    float   frustumDepth = _far - _near;
    float   oneOverDepth = 1 / frustumDepth;

    _projection = Matrix4f();

    for (int x = 1; x <= 4; x++)
        for (int y = 1; y <= 4; y++)
            _projection(x, y) = 0;

    _projection(2, 2) = 1 / tan(0.5f * _fov);
    _projection(1, 1) = -1 * _projection(2, 2) / _ratio;
    _projection(3, 3) = _far * oneOverDepth;
    _projection(4, 3) = (-_far * _near) * oneOverDepth;
    _projection(3, 4) = 1;
    _projection(4, 4) = 0;
}