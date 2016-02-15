#include "MiniEngine/CommandList.h"
#include "MiniEngine/RenderSystem.h"

using namespace MiniEngine;

CommandList::CommandList(RenderSystem &system, RenderTarget *target) : _system(system), _target(target), _previousMaterialId(-1), _previousMaterialType(-1), _bundle(false)
{}

CommandList::~CommandList()
{}

bool CommandList::setMaterialPipeline(DWORD64 type)
{
    if (_previousMaterialType == type)
        return (true);

    _previousMaterialType = type;

    GraphicPipeline *pipeline = _system.getGraphicPipeline(type);

    if (!pipeline)
        return (false);

    return (setPipeline(*pipeline));
}

RenderTarget &CommandList::getRenderTarget()
{
    return (*_target);
}

RenderSystem &CommandList::getRenderSystem()
{
    return (_system);
}
