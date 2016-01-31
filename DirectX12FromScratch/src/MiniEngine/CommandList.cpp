#include "MiniEngine/CommandList.h"

using namespace MiniEngine;

CommandList::CommandList(RenderSystem &system, RenderTarget *target, GraphicPipeline &pipeline) : _system(system), _target(target), _pipeline(pipeline)
{}

CommandList::~CommandList()
{}

RenderTarget &CommandList::getRenderTarget()
{
    return (*_target);
}