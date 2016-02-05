# include "MiniEngine/BasicSceneManager.h"

using namespace MiniEngine;

BasicSceneManager::BasicSceneManager(RenderSystem &system) : SceneManager(system)
{
    _rootNode = new SceneNode(*this);
}

BasicSceneManager::~BasicSceneManager()
{}

bool BasicSceneManager::render(Camera &camera, CommandList &commandList)
{
    if (_needUpdate)
    {
        _needUpdate = false;

        if (!_lightsConstantBuffer->updateLights(_lights))
            return (false);
    }

    if (!commandList.bindLightsCBV(*_lightsConstantBuffer))
        return (false);

    return (_rootNode->render(camera, commandList));
}
