#include <iostream>
#include <DirectXMath.h>
#include "MiniEngine/D3D12/D3D12CommandList.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12Device.h"

using namespace MiniEngine;

D3D12CommandList::D3D12CommandList(D3D12RenderSystem &system, D3D12RenderTarget &target, D3D12GraphicPipeline &pipeline) : CommandList(system, target, pipeline), _system(system), _pipeline(pipeline), _allocator(nullptr), _list(nullptr)
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

void D3D12CommandList::setCameraMatrix(Matrix4f const &view, Matrix4f const &projection)
{
    struct CameraMatrix
    {
        DirectX::XMFLOAT4X4  view;
        DirectX::XMFLOAT4X4  projection;
    }   camera;

    for (unsigned int x = 0; x < 4; x++)
        for (unsigned int y = 0; y < 4; y++)
            camera.view.m[x][y] = view(x + 1, y + 1);

    for (unsigned int x = 0; x < 4; x++)
        for (unsigned int y = 0; y < 4; y++)
            camera.projection.m[x][y] = projection(x + 1, y + 1);

    _target.getCameraBuffer()->update(*this, sizeof(camera), &camera);
}

void D3D12CommandList::afterCameraRender()
{
    _target.getCameraBuffer()->afterUpdate(*this);
}

ID3D12GraphicsCommandList *D3D12CommandList::getNative()
{
    return (_list);
}