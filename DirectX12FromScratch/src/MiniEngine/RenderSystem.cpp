#include "MiniEngine/RenderSystem.h"
#include "MiniEngine/RenderTarget.h"
#include "MiniEngine/Material.h"

using namespace MiniEngine;

RenderSystem::RenderSystem()
{}

RenderSystem::~RenderSystem()
{
    clear();

    for (auto &&pair : _pipelines)
    {
        delete pair.second;
        pair.second = nullptr;
    }

    for (auto &&pair : _materials)
    {
        delete pair.second;
        pair.second = nullptr;
    }
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

Material *RenderSystem::getMaterial(unsigned int id)
{
    if (_materials.find(id) != _materials.end())
        return (_materials.at(id));

    return (nullptr);
}

bool RenderSystem::registerMaterial(Material *material)
{
    if (_pipelines.find(material->getFlags()) == _pipelines.end())
    {
        GraphicPipeline *pipeline = createGraphicPipeline(*material);

        if (!pipeline)
        {
            delete material;
            return (false);
        }

        _pipelines[material->getFlags()] = pipeline;
    }

    _materials[material->getId()] = material;
    return (true);
}

GraphicPipeline *RenderSystem::getGraphicPipeline(DWORD64 type)
{
    if (_pipelines.find(type) == _pipelines.end())
        return (nullptr);

    return (_pipelines.at(type));
}

bool RenderSystem::render()
{
    for (auto &&target : _targets)
    {
        if (!target->render())
            return (false);
    }

    return (true);
}