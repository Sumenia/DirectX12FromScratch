#include "MiniEngine/Light.h"
#include "MiniEngine/SceneManager.h"

using namespace MiniEngine;

Light::Light(SceneManager &manager) : _manager(manager),
    _ambient(0.0f, 0.0f, 0.0f), _diffuse(0.0f, 0.0f, 0.0f), _specular(0.0f, 0.0f, 0.0f), 
    _constant(1.0f), _linear(0.0f), _quadratic(0.0f)
{}

Light::~Light()
{
    _manager.removeLight(this);
}

void Light::needUpdate()
{
    _manager.updateLightBuffer();
}

Vector3f &Light::getAmbient()
{
    return (_ambient);
}

void Light::setAmbient(Vector3f const &ambient)
{
    _ambient = ambient;
}

Vector3f &Light::getDiffuse()
{
    return (_diffuse);
}

void Light::setDiffuse(Vector3f const &diffuse)
{
    _diffuse = diffuse;
}

Vector3f &Light::getSpecular()
{
    return (_specular);
}

void Light::setSpecular(Vector3f const &specular)
{
    _specular = specular;
}

void	Light::setConstant(float constant)
{
	_constant = constant;
}

float	Light::getConstant()
{
	return (_constant);
}

void	Light::setLinear(float linear)
{
	_linear = linear;
}

float	Light::getLinear()
{
	return (_linear);
}

void	Light::setQuadratic(float quadratic)
{
	_quadratic = quadratic;
}

float	Light::getQuadratic()
{
	return (_quadratic);
}

/* Directional + Spot */
Vector3f	Light::getDirection() const
{
	return _direction;
}

/* Spot */
float Light::getOuterCutOff() const
{
	return _outerCutOffAngle;
}

float Light::getInnerCutOff() const
{
	return _innerCutOffAngle;
}

Vector3f Light::getDirection() const
{
	return _direction;
}

/* Point */
float Light::getRange() const
{
	return _range;
}
