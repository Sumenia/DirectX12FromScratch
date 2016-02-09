#include "MiniEngine/DirectionalLight.h"

using namespace MiniEngine;

DirectionalLight::DirectionalLight(SceneManager &manager, Vector3f direction) : Light(manager)
{
	_direction = direction;
}

DirectionalLight::~DirectionalLight()
{
}

void	DirectionalLight::setDirection(Vector3f direction)
{
	_direction = direction;
}