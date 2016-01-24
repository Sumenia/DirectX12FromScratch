#include <iostream>
#include "MiniEngine/D3D12/D3D12Device.h"
#include "MiniEngine/D3D12/D3D12CommandQueue.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"

using namespace MiniEngine;

D3D12CommandQueue::D3D12CommandQueue(D3D12RenderSystem &system) : _system(system), _queue(nullptr)
{}

D3D12CommandQueue::~D3D12CommandQueue()
{
    delete (_queue);
}

bool D3D12CommandQueue::init()
{
    HRESULT                     result;
    D3D12_COMMAND_QUEUE_DESC    queueDesc = {};

    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    result = _system.getDevice()->getNative()->CreateCommandQueue(&queueDesc, __uuidof(ID3D12CommandQueue), (void**)&_queue);

    if (FAILED(result))
    {
        std::cout << "Can't create CommandQueue" << std::endl;
        _queue = nullptr;

        return (false);
    }

    return (true);
}

ID3D12CommandQueue *D3D12CommandQueue::getNative()
{
    return (_queue);
}