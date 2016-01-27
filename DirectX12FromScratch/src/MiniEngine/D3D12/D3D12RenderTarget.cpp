#include "MiniEngine/D3D12/D3D12RenderTarget.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"

using namespace MiniEngine;

D3D12RenderTarget::D3D12RenderTarget(D3D12RenderSystem &system) : RenderTarget(system), _system(system)
{
}

D3D12RenderTarget::~D3D12RenderTarget()
{
}
