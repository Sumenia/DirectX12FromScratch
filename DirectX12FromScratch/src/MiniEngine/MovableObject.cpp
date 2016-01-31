#include "MiniEngine/MovableObject.h"

using namespace MiniEngine;

MovableObject::MovableObject() : _parent(nullptr), _needUpdate(true)
{}

MovableObject::~MovableObject()
{}

void MovableObject::setParent(SceneNode *parent)
{
    _parent = parent;
}

SceneNode *MovableObject::getParent()
{
    return (_parent);
}

void MovableObject::needUpdate()
{
    _needUpdate = true;
}