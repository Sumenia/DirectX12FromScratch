#include "SpotLight.h"

using namespace MiniEngine;

SpotLight::SpotLight(SceneManager &manager, Vector3f direction, float innerCutOffAngle, float outterCutOffAngle) : Light(manager), _direction(direction), _innerCutOffAngle(innerCutOffAngle), _outerCutOffAngle(outterCutOffAngle)
{
}

SpotLight::~SpotLight()
{
}

float SpotLight::getOuterCutOff() const
{
	return _outerCutOffAngle;
}

void SpotLight::setOuterCutOff(float outerCutOff)
{
	_outerCutOffAngle = outerCutOff;
}

float SpotLight::getInnerCutOff() const
{
	return _innerCutOffAngle;
}

void SpotLight::setInnerCutOff(float cutOff)
{
	_innerCutOffAngle = cutOff;
}

Vector3f SpotLight::getDirection() const
{
	return _direction;
}

void SpotLight::setDirection(Vector3f direction)
{
	_direction = direction;
}
