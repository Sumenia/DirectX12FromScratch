#include "MiniEngine/PointLight.h"

using namespace MiniEngine;

PointLight::PointLight(SceneManager &manager) : Light(manager)
{}

PointLight::PointLight(SceneManager &manager, float range) : Light(manager)
{
	_range = range;
}

PointLight::~PointLight()
{
}

void PointLight::setRange(float intensity)
{
	_range = intensity;
}

Light::Type	PointLight::getType() const
{
	return Light::POINT;
}