#include "MiniEngine/CommandList.h"

using namespace MiniEngine;

CommandList::CommandList(RenderSystem &system, RenderTarget *target) : _system(system), _target(target)
{}

CommandList::~CommandList()
{}

RenderTarget &CommandList::getRenderTarget()
{
    return (*_target);
}

RenderSystem &CommandList::getRenderSystem()
{
    return (_system);
}