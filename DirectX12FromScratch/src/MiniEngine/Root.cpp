#include "MiniEngine/Root.h"
#include "MiniEngine/BasicSceneManager.h"

using namespace MiniEngine;

Root::Root() : _renderSystem(nullptr)
{}

Root::~Root()
{
    delete _renderSystem;
}

void Root::setRenderSystem(RenderSystem *renderSystem)
{
    delete _renderSystem;
    _renderSystem = renderSystem;

    if (!_renderSystem->init())
    {
        delete _renderSystem;
        _renderSystem = nullptr;
    }
}

RenderSystem *Root::getRenderSystem() const
{
    return (_renderSystem);
}

SceneManager *Root::createSceneManager(SceneManagerType type)
{
    SceneManager    *sceneManager = nullptr;

    if (type == BASIC)
        sceneManager = new BasicSceneManager(*_renderSystem);

    if (!sceneManager || !sceneManager->init())
    {
        delete sceneManager;
        return (nullptr);
    }

    return (sceneManager);
}