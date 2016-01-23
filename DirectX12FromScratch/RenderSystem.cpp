#include "RenderSystem.h"

using namespace MiniEngine;

RenderSystem::RenderSystem()
{}

RenderSystem::~RenderSystem()
{
    while (_targets.size())
    {
        delete _targets.front();
        _targets.pop_front();
    }
}

bool RenderSystem::init()
{
    return (true);
}

void RenderSystem::addRenderTarget(RenderTarget *target)
{
    _targets.push_back(target);
}