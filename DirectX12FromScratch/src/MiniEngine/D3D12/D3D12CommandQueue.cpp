#include <iostream>
#include "MiniEngine/D3D12/D3D12Device.h"
#include "MiniEngine/D3D12/D3D12CommandQueue.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12Fence.h"

using namespace MiniEngine;

D3D12CommandQueue::D3D12CommandQueue(D3D12RenderSystem &system) : _system(system), _queue(nullptr)
{}

D3D12CommandQueue::~D3D12CommandQueue()
{
    if (_queue)
        _queue->Release();
    _queue = nullptr;
}

D3D12CommandList *D3D12CommandQueue::createCommandList(RenderTarget &target, GraphicPipeline &pipeline)
{
    return (new D3D12CommandList(_system, dynamic_cast<D3D12RenderTarget&>(target), dynamic_cast<D3D12GraphicPipeline&>(pipeline)));
}

bool D3D12CommandQueue::wait(Fence &fence)
{
    return (wait(dynamic_cast<D3D12Fence&>(fence)));
}

bool D3D12CommandQueue::wait(D3D12Fence &fence)
{
    HRESULT         result;
    const UINT64    fenceValue = fence.getValue();

    result = _queue->Signal(fence.getNative(), fenceValue);

    if (FAILED(result))
    {
        std::cout << "Can't set signal on fence" << std::endl;
        return (false);
    }

    fence.incValue();

    if (fence.getNative()->GetCompletedValue() < fenceValue)
    {
        result = fence.getNative()->SetEventOnCompletion(fenceValue, fence.getEvent());

        if (FAILED(result))
        {
            std::cout << "Can't set event on completion" << std::endl;
            return (false);
        }

        WaitForSingleObject(fence.getEvent(), INFINITE);
    }

    return (true);
}

bool D3D12CommandQueue::executeCommandLists(unsigned int nb, CommandList *tmpList)
{
    D3D12CommandList    *list = dynamic_cast<D3D12CommandList*>(tmpList);
    ID3D12CommandList   **commandLists = new ID3D12CommandList*[nb];

    for (unsigned int i = 0; i < nb; i++)
        commandLists[i] = list[i].getNative();

    _queue->ExecuteCommandLists(nb, commandLists);

    delete[] commandLists;

    return (true);
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