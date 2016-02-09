#include "PointLight.h"

using namespace MiniEngine;

PointLight::PointLight(SceneManager &manager, float intancity) : Light(manager), _intancity(intancity)
{
}

PointLight::~PointLight()
{
}

void PointLight::setIntansity(float intensity)
{
	_intancity = intensity;
}


float PointLight::getIntansity() const
{
	return _intancity;
}

