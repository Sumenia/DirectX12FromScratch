#include <iostream>
#include "MiniEngine/D3D12/D3D12CommandList.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12Device.h"

using namespace MiniEngine;

D3D12CommandList::D3D12CommandList(D3D12RenderSystem &system, D3D12GraphicPipeline &pipeline) : CommandList(system, pipeline), _system(system), _pipeline(pipeline), _allocator(nullptr), _list(nullptr)
{}

D3D12CommandList::~D3D12CommandList()
{
    if (_list)
        _list->Release();

    _list = nullptr;

    if (_allocator)
        _allocator->Release();

    _allocator = nullptr;
}

bool D3D12CommandList::init()
{
    HRESULT             result;

    result = _system.getDevice()->getNative()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&_allocator);

    if (FAILED(result))
    {
        std::cout << "Can't create Command allocator" << std::endl;
        return (false);
    }

    result = _system.getDevice()->getNative()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _allocator, _pipeline.getNative(), __uuidof(ID3D12GraphicsCommandList), (void**)&_list);

    if (FAILED(result))
    {
        std::cout << "Can't create Command list" << std::endl;
        return (false);
    }

    return (end());
}

bool D3D12CommandList::reset()
{
    HRESULT     result;

    result = _allocator->Reset();

    if (FAILED(result))
        return (false);

    result = _list->Reset(_allocator, _pipeline.getNative());

    return (!FAILED(result));
}

bool D3D12CommandList::begin()
{
    return (true);
}

bool D3D12CommandList::end()
{
    HRESULT     result;

    result = _list->Close();

    return (!FAILED(result));
}

ID3D12GraphicsCommandList *D3D12CommandList::getNative()
{
    return (_list);
}