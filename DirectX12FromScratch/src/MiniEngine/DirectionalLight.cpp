#include "MiniEngine/DirectionalLight.h"

using namespace MiniEngine;

DirectionalLight::DirectionalLight(SceneManager &manager) : Light(manager)
{}

DirectionalLight::DirectionalLight(SceneManager &manager, Vector3f direction) : Light(manager)
{
	_direction = direction;
}

DirectionalLight::~DirectionalLight()
{
}

Light::Type	DirectionalLight::getType() const
{
	return Light::DIRECTIONAL;
}

void	DirectionalLight::setDirection(Vector3f direction)
{
	_direction = direction;
}