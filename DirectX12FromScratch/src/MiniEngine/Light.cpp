#include "MiniEngine/Light.h"
#include "MiniEngine/SceneManager.h"

using namespace MiniEngine;

Light::Light(SceneManager &manager) : _manager(manager),
    _type(POINT), 
    _direction(0.0f, 0.0f, 0.0f), 
    _cutOff(0.0f), _outerCutOff(0.0f), 
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