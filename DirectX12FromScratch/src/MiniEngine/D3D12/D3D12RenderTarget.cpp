#include "MiniEngine/D3D12/D3D12RenderTarget.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"

using namespace MiniEngine;

D3D12RenderTarget::D3D12RenderTarget(D3D12RenderSystem &system) : RenderTarget(system), _system(system), _pipeline(nullptr)
{
    _pipeline = new D3D12GraphicPipeline(_system);
}

D3D12RenderTarget::~D3D12RenderTarget()
{
    delete _pipeline;
    _pipeline = nullptr;
}

D3D12GraphicPipeline *D3D12RenderTarget::getGraphicPipeline()
{
    return (_pipeline);
}