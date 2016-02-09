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

float MiniEngine::Light::getCosineLightAngle()
{
	float  cosine = cos(_cutOff);
	std::cout << cosine << std::endl;
	return cosine;
}

float MiniEngine::Light::getCosineBigAngle()
{
	float  cosine = cos(_outerCutOff);
	std::cout << cosine << std::endl;
	return cosine;
}
