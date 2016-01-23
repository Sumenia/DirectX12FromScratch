#include "D3D12RenderSystem.h"

using namespace MiniEngine;

D3D12RenderSystem::D3D12RenderSystem()
{}

D3D12RenderSystem::~D3D12RenderSystem()
{}

bool D3D12RenderSystem::init()
{
    _device = D3D12Device::create();

    if (!_device)
        return (false);

    return (true);
}