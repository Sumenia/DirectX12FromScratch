# include "MiniEngine/BasicSceneManager.h"

using namespace MiniEngine;

BasicSceneManager::BasicSceneManager() : SceneManager()
{
    _rootNode = new SceneNode(*this);
}

BasicSceneManager::~BasicSceneManager()
{}

bool BasicSceneManager::render(Camera &camera, CommandList &commandList)
{
    return (_rootNode->render(camera, commandList));
}
