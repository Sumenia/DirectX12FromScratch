#include "MiniEngine/SceneNode.h"
#include "MiniEngine/Camera.h"
#include "MiniEngine/CommandList.h"
#include "MiniEngine/RenderTarget.h"

using namespace MiniEngine;

SceneNode::SceneNode(SceneManager &manager, MovableObject *object) : _manager(manager), _parent(nullptr), _obj(object), _scaling(1.0f, 1.0f, 1.0f), _rotation(Quatf::fromAxisRot(Vector3f(0, 0, 0), 0)), _needUpdate(true), _modelConstantBuffer(nullptr)
{}

SceneNode::~SceneNode()
{
    delete _modelConstantBuffer;
    _modelConstantBuffer = nullptr;

    while (_childs.size())
    {
        delete _childs.front();
        _childs.pop_front();
    }
}

SceneNode *SceneNode::createChild(MovableObject *object)
{
    return (addChild(new SceneNode(_manager, object)));
}

SceneNode *SceneNode::addChild(SceneNode *node)
{
    _childs.push_back(node);

    node->setParent(this);
    node->needUpdate();

    return (node);
}

void SceneNode::attachObject(MovableObject *obj)
{
    _obj.reset(obj);

    if (!_obj)
    {
        delete _modelConstantBuffer;
        _modelConstantBuffer = nullptr;
    }
    else
    {
        _obj->setParent(this);
    }
}

SceneNode *SceneNode::getParent()
{
    return (_parent);
}

bool SceneNode::render(Camera &camera, CommandList &commandList)
{
    std::shared_ptr<RenderableObject>   object = std::dynamic_pointer_cast<RenderableObject>(_obj);

    if (object)
    {
        if (_needUpdate)
            update();

        if (!_modelConstantBuffer)
        {
            _modelConstantBuffer = commandList.getRenderSystem().createConstantBuffer(64 + 36, commandList.getRenderTarget().getFrameCount());
            update();
        }

        if (!_modelConstantBuffer)
        {
            std::cout << "Can't create constant buffer for this model" << std::endl;
            return (false);
        }

        if (!commandList.bindModelCBV(*_modelConstantBuffer))
            return (false);

        if (!object->render(camera, commandList))
            return (false);
    }

    for (auto &&child : _childs)
        if (!child->render(camera, commandList))
            return (false);

    return (true);
}

Vector3f &SceneNode::getDerivedPosition()
{
    if (_needUpdate)
        update();

    return (_derivedPosition);
}

Quatf &SceneNode::getDerivedRotation()
{
    if (_needUpdate)
        update();

    return (_derivedRotation);
}

Vector3f &SceneNode::getDerivedScaling()
{
    if (_needUpdate)
        update();

    return (_derivedScaling);
}

Matrix4f &SceneNode::getTransformationMatrix()
{
    if (_needUpdate)
        update();

    return (_transform);
}

void MiniEngine::SceneNode::rotate(float w, Vector3f &v, TransformSpace space)
{
	Quatf q = Quatf::fromAxisRot(v, w);
    q.normalize();

    if (space == TS_LOCAL)
        _rotation = _rotation * q;
    else if (space == TS_PARENT)
        _rotation = q * _rotation;
    else if (space == TS_WORLD)
        _rotation = _rotation - getDerivedRotation().inverse() * q * getDerivedRotation();

	needUpdate();
}

void MiniEngine::SceneNode::translate(Vector3f &v, TransformSpace space)
{
    if (space == TS_LOCAL)
        _position += _rotation.transform() * v;
    else if (space == TS_PARENT)
        _position += v;
    else if (space == TS_WORLD)
    {
        if (_parent)
            _position += (_parent->getDerivedRotation().inverse().transform() * v) / _parent->getDerivedScaling();
        else
            _position += v;
    }

    needUpdate();
}

void MiniEngine::SceneNode::scale(Vector3f &v)
{
    _scaling *= v;
    needUpdate();
}

void SceneNode::needUpdate()
{
    _needUpdate = true;

    for (auto &&child : _childs)
        child->needUpdate();

    if (_obj)
        _obj->needUpdate();
}

void SceneNode::setParent(SceneNode *node)
{
    _parent = node;
}

void SceneNode::update()
{
    if (_parent)
    {
        _derivedPosition = _parent->getDerivedRotation().transform() * (_parent->getDerivedScaling() * _position) + _parent->getDerivedPosition();
        _derivedRotation = _parent->getDerivedRotation() * _rotation;
        _derivedScaling = _parent->getDerivedScaling() * _scaling;
    }
    else
    {
        _derivedPosition = _position;
        _derivedRotation = _rotation;
        _derivedScaling = _scaling;
    }

    _needUpdate = false;
    _transform = Matrix4f::createTranslation(_derivedPosition.x, _derivedPosition.y, _derivedPosition.z) * _derivedRotation.transform() * Matrix4f::createScale(_derivedScaling.x, _derivedScaling.y, _derivedScaling.z);

    if (_modelConstantBuffer)
        if (!_modelConstantBuffer->updateModelMatrix(_transform))
        {
            delete _modelConstantBuffer;
            _modelConstantBuffer = nullptr;
        }
}