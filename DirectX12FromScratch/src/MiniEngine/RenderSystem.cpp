#include "MiniEngine/RenderSystem.h"
#include "MiniEngine/RenderTarget.h"

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
	if (target->init())
		_targets.push_back(target);
	else
		delete target;
}

void RenderSystem::clear()
{
    while (_targets.size())
    {
        delete _targets.front();
        _targets.pop_front();
    }
}