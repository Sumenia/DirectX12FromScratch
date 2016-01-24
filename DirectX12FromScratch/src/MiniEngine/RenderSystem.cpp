#include "MiniEngine/RenderSystem.h"

using namespace MiniEngine;

RenderSystem::RenderSystem()
{}

RenderSystem::~RenderSystem()
{
    clear();
}

bool RenderSystem::init()
{
    return (true);
}

void RenderSystem::addRenderTarget(RenderTarget *target)
{
    _targets.push_back(target);
}

void RenderSystem::clear()
{
    while (_targets.size())
    {
        delete _targets.front();
        _targets.pop_front();
    }
}