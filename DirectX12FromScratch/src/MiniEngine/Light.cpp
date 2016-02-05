#include "MiniEngine/Light.h"
#include "MiniEngine/SceneManager.h"

using namespace MiniEngine;

Light::Light(SceneManager &manager) : _manager(manager)
{}

Light::~Light()
{
    _manager.removeLight(this);
}

void Light::needUpdate()
{
    _manager.updateLightBuffer();
}