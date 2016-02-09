#include "DirectionalLight.h"

using namespace MiniEngine;

DirectionalLight::DirectionalLight(SceneManager &manager, float angle) : Light(manager), _angle(angle)
{
}

DirectionalLight::~DirectionalLight()
{
}

float DirectionalLight::getAngle() const
{
	return _angle;
}

void DirectionalLight::setAngle()
{
}
