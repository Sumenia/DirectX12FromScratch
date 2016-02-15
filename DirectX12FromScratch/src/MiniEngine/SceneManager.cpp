#include "MiniEngine/SceneManager.h"
#include "MiniEngine/Light.h"
#include "MiniEngine/RenderSystem.h"
#include "MiniEngine/PointLight.h"
#include "MiniEngine/DirectionalLight.h"
#include "MiniEngine/SpotLight.h"

using namespace MiniEngine;

SceneManager::SceneManager(RenderSystem &system) : _system(system), _rootNode(nullptr), _lightsConstantBuffer(nullptr), _needUpdate(true)
{}

SceneManager::~SceneManager()
{
    _lights.clear();

    delete _lightsConstantBuffer;
    _lightsConstantBuffer = nullptr;

    delete _rootNode;
    _rootNode = nullptr;
}

bool SceneManager::init()
{
    _lightsConstantBuffer = _system.createConstantBuffer(96 * MAX_LIGHTS);
    return (!!_lightsConstantBuffer);
}

SceneNode *SceneManager::getRootNode()
{
    return (_rootNode);
}

Camera *SceneManager::createCamera(SceneNode *node)
{
    Camera  *camera = new Camera(*this);

    if (!node)
        node = _rootNode;

    node->addChild(camera);

    return (camera);
}

Light *SceneManager::createLight(Light::Type type, SceneNode *node)
{
    Light   *light = nullptr;

    if (_lights.size() == MAX_LIGHTS)
        return (nullptr);

    if (type == Light::POINT)
        light = new PointLight(*this);
    else if (type == Light::SPOT)
        light = new SpotLight(*this);
    else if (type == Light::DIRECTIONAL)
        light = new DirectionalLight(*this);

    if (!light)
        return (nullptr);

    _lights.push_back(light);
    _needUpdate = true;

    if (!node)
        node = _rootNode;

    node->createChild(light);

    return (light);
}

void SceneManager::removeLight(Light *light)
{
    for (auto it = _lights.begin(); it != _lights.end(); it++)
    {
        if (*it = light)
        {
            _needUpdate = true;
            _lights.erase(it);

            return;
        }
    }
}

void SceneManager::updateLightBuffer()
{
    _needUpdate = true;
}

std::list<Light*> const SceneManager::getLights() const
{
    return (_lights);
}