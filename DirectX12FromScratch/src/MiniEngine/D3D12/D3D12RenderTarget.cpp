#include "MiniEngine/D3D12/D3D12RenderTarget.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"

using namespace MiniEngine;

D3D12RenderTarget::D3D12RenderTarget(D3D12RenderSystem &system) : RenderTarget(system), _system(system)
{
}

bool D3D12RenderTarget::init()
{
	ZeroMemory(&_rtvHeapDesc, sizeof(_rtvHeapDesc));
	_rtvHeapDesc.NumDescriptors = _system.get;
	_rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	_rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	return (true);
}

D3D12RenderTarget::~D3D12RenderTarget()
{
}
