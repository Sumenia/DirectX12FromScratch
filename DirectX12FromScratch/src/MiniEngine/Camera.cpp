
#include "MiniEngine/Geometry.h"
#include "MiniEngine/Camera.h"
#include "MiniEngine/SceneManager.h"

using namespace MiniEngine;

Camera::Camera(SceneManager &manager) : SceneNode(manager), _fov(70.0f), _ratio(16.0f / 9.0f), _near(0.01f), _far(100.0f)
{}

Camera::~Camera()
{}

bool Camera::render(CommandList &commandList)
{
    bool    result;

    if (_needUpdate)
    {
        _worldView = getTransformationMatrix() * _view;
        _needUpdate = false;
    }

    commandList.setCameraMatrix(_worldView, _projection);
    result = _manager.render(*this, commandList);
    commandList.afterCameraRender();

    return (result);
}

void Camera::lookAt(const Vector3f &eye, const Vector3f &target, const Vector3f &up)
{
    _view = Matrix4f::createLookAt(eye, target, up);
    _needUpdate = true;
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

    _projection(2, 2) = 1 / tan(0.5f * (_fov * M_PI / 180.0f));
    _projection(1, 1) = 1 * _projection(2, 2) / _ratio;
    _projection(3, 3) = -_far * oneOverDepth;
    _projection(3, 4) = (-_far * _near) * oneOverDepth;
    _projection(4, 3) = -1;
    _projection(4, 4) = 0;
}