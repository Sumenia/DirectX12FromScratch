#include "MiniEngine/CommandList.h"

using namespace MiniEngine;

CommandList::CommandList(RenderSystem &system, GraphicPipeline &pipeline) : _system(system), _pipeline(pipeline)
{}

CommandList::~CommandList()
{}