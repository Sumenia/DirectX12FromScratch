#include "MiniEngine/Root.h"

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