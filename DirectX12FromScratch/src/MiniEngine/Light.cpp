#include "MiniEngine/Light.h"
#include "MiniEngine/SceneManager.h"

using namespace MiniEngine;

Light::Light(SceneManager &manager) : _manager(manager),
    _type(SPOT), 
    _direction(-0.1f, -0.1f, 0.0f), 
    _cutOff(1.0f), _outerCutOff(5.0f), 
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

Light::Type Light::getType()
{
    return (_type);
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

float Light::getCutOff()
{
	return	(_cutOff);
}
void Light::setCutOff(float cutOff) 
{
	_cutOff = cutOff;
}

float Light::getOuterCutOff()
{
	return (_outerCutOff);
}

void Light::setOuterCutOff(float outerCutOff) 
{
	_outerCutOff = outerCutOff;
}


Vector3f &Light::getDirection()
{
	return (_direction);
}

void Light::setDirection(Vector3f const &direction)
{
	_direction = direction;
}

void MiniEngine::Light::setConstant(float constant)
{
	_constant = constant;
}

float MiniEngine::Light::getConstant()
{
	return (_constant);
}

void MiniEngine::Light::setLinear(float linear)
{
	_linear = linear;
}

float MiniEngine::Light::getLinear()
{
	return (_linear);
}

void MiniEngine::Light::setQuadratic(float quadratic)
{
	_quadratic = quadratic;
}

float MiniEngine::Light::getQuadratic()
{
	return (_quadratic);
}