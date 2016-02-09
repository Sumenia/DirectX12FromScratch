#include "MiniEngine/Spotlight.h"

using namespace MiniEngine;

SpotLight::SpotLight(SceneManager &manager) : Light(manager)
{}

SpotLight::SpotLight(SceneManager &manager, Vector3f direction, float innerCutOffAngle, float outterCutOffAngle) : Light(manager)
{
	_direction = direction;
	_innerCutOffAngle = innerCutOffAngle;
	_outerCutOffAngle = outterCutOffAngle;
}

SpotLight::SpotLight(SceneManager &manager, Vector3f direction, float innerCutOffAngle) : SpotLight::SpotLight(manager, direction, innerCutOffAngle, innerCutOffAngle)
{
}

SpotLight::~SpotLight()
{
}

void SpotLight::setOuterCutOff(float outerCutOff)
{
	_outerCutOffAngle = outerCutOff;
}


void SpotLight::setInnerCutOff(float cutOff)
{
	_innerCutOffAngle = cutOff;
}

void SpotLight::setDirection(Vector3f direction)
{
	_direction = direction;
}

void SpotLight::setRange(float intensity)
{
    _range = intensity;
}

Light::Type	SpotLight::getType() const
{
	return Light::SPOT;
}
