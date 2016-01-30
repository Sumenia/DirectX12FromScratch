
#include "MiniEngine/D3D12/D3D12ConstantBuffer.h"
#include "MiniEngine/D3D12/D3D12Device.h"

using namespace MiniEngine;

MiniEngine::D3D12ConstantBuffer::D3D12ConstantBuffer(D3D12RenderSystem &system) : ConstantBuffer(system), _system(system)
{
}

D3D12ConstantBuffer::~D3D12ConstantBuffer()
{

}

bool MiniEngine::D3D12ConstantBuffer::init(unsigned int size)
{
	CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC constantBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(size);

	HRESULT result = _system.getDevice()->getNative()->CreateCommittedResource(
														&defaultHeapProperties,
														D3D12_HEAP_FLAG_NONE,
														&constantBufferDesc,
														D3D12_RESOURCE_STATE_GENERIC_READ,
														nullptr,
														IID_PPV_ARGS(&_constantBuffer));
	if (FALSE(result))
		return (false);
	return (true);
}
