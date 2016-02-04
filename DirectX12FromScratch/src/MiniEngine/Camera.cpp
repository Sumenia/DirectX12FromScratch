
#include "MiniEngine/Geometry.h"
#include "MiniEngine/Camera.h"
#include "MiniEngine/SceneManager.h"
#include "MiniEngine/CommandList.h"
#include "MiniEngine/RenderTarget.h"

using namespace MiniEngine;

Camera::Camera(SceneManager &manager) : SceneNode(manager), _fov(70.0f), _ratio(16.0f / 9.0f), _near(0.01f), _far(1000.0f), _cameraConstantBuffer(nullptr)
{}

Camera::~Camera()
{
    delete _cameraConstantBuffer;
    _cameraConstantBuffer = nullptr;
}

bool Camera::render(CommandList &commandList)
{
    if (_needUpdate)
        update();

    if (!_cameraConstantBuffer)
    {
        _cameraConstantBuffer = commandList.getRenderSystem().createConstantBuffer(64 + 64, commandList.getRenderTarget().getFrameCount());
        update();
    }

    if (!_cameraConstantBuffer)
    {
        std::cout << "Can't create constant buffer for this camera" << std::endl;
        return (false);
    }

    if (!commandList.bindCameraCBV(*_cameraConstantBuffer))
        return (false);

    return (_manager.render(*this, commandList));
}

void Camera::lookAt(const Vector3f &eye, const Vector3f &target, const Vector3f &up)
{
	_position = eye;
	_rotation = Quatf::fromMatrix(Matrix4f::createTranslation(-eye.x, -eye.y, -eye.z) * Matrix4f::createLookAt(eye, target, up).inverse());
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

    _projection(2, 2) = (float) (1 / tan(0.5f * (_fov * M_PI / 180.0f)));
    _projection(1, 1) = 1 * _projection(2, 2) / _ratio;
    _projection(3, 3) = -_far * oneOverDepth;
    _projection(3, 4) = (-_far * _near) * oneOverDepth;
    _projection(4, 3) = -1;
    _projection(4, 4) = 0;

    update();
}

void Camera::update()
{
    SceneNode::update();

    if (_cameraConstantBuffer)
        if (!_cameraConstantBuffer->updateCameraMatrix(getTransformationMatrix().inverse(), _projection))
        {
            delete _cameraConstantBuffer;
            _cameraConstantBuffer = nullptr;
        }
}