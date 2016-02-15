#include "MiniEngine/Viewport.h"

using namespace MiniEngine;

Viewport::Viewport(Vector2f position, Vector2f size, Vector2f percent) : _camera(nullptr), _position(position), _size(size), _percent(percent)
{}

Viewport::~Viewport()
{
    _camera = nullptr;
}

void Viewport::attachCamera(Camera *camera)
{
    _camera = camera;

    _camera->setRatio(_size.x / _size.y);
}

bool Viewport::render(CommandList &commandList)
{
    if (_camera)
        return (_camera->render(commandList));

    return (true);
}

Vector2f const &Viewport::getPosition() const
{
    return (_position);
}

Vector2f const &Viewport::getSize() const
{
    return (_size);
}

Vector2f const &Viewport::getPercent() const
{
	return (_percent);
}

void			Viewport::setSize(Vector2f size)
{
	_size = size;
	if (_camera)
		_camera->setRatio(_size.x / _size.y);
}