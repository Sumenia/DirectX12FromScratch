#include "MiniEngine/SceneManager.h"

using namespace MiniEngine;

SceneManager::SceneManager() : _rootNode(nullptr)
{}

SceneManager::~SceneManager()
{
    delete _rootNode;
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