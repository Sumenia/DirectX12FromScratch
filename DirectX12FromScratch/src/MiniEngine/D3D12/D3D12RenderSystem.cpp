#include <iostream>
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12Device.h"
#include "MiniEngine/D3D12/D3D12CommandQueue.h"
#include "MiniEngine/D3D12/D3D12RenderWindow.h"

using namespace MiniEngine;

D3D12RenderSystem::D3D12RenderSystem()
{}

D3D12RenderSystem::~D3D12RenderSystem()
{
    clear();

    delete _commandQueue;
    delete _rootSignature;
    delete _device;
    _factory = nullptr;
}

bool D3D12RenderSystem::init()
{
    return (
        initDevice()
        && initRootSignature()
        && initCommandQueue()
        && initFence()
        && _commandQueue->wait(*_fence)
    );
}

bool D3D12RenderSystem::initDevice()
{
    HRESULT                 result;

    result = CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&_factory);

    if (FAILED(result))
    {
        std::cout << "Failed to create DXGI Factory" << std::endl;
        return (false);
    }

    _device = D3D12Device::create();

    if (!_device)
    {
        std::cout << "Failed to create device" << std::endl;
        return (false);
    }

    return (true);
}

bool D3D12RenderSystem::initCommandQueue()
{
    _commandQueue = new D3D12CommandQueue(*this);
    return (_commandQueue->init());
}

bool D3D12RenderSystem::initFence()
{
    _fence = new D3D12Fence(*this);
    return (_fence->init());
}

bool D3D12RenderSystem::initRootSignature()
{
    _rootSignature = new D3D12RootSignature();
    return (_rootSignature->init(*this));
}

IDXGIFactory4 *D3D12RenderSystem::getFactory()
{
    return (_factory);
}

D3D12Device *D3D12RenderSystem::getDevice()
{
    return (_device);
}

D3D12CommandQueue *D3D12RenderSystem::getCommandQueue()
{
    return (_commandQueue);
}

D3D12Fence *D3D12RenderSystem::getFence()
{
    return (_fence);
}

D3D12RootSignature *D3D12RenderSystem::getRootSignature()
{
    return (_rootSignature);
}

D3D12ConstantBuffer *D3D12RenderSystem::createConstantBuffer(unsigned int size, unsigned int nb)
{
    D3D12ConstantBuffer *buffer = new D3D12ConstantBuffer(*this);

    if (!buffer->init(size, nb))
    {
        delete buffer;
        return (nullptr);
    }

    return (buffer);
}

D3D12RenderWindow *D3D12RenderSystem::createRenderWindow(Window *window)
{
    return (new D3D12RenderWindow(*this, window));
}

D3D12HLSLShader *D3D12RenderSystem::createHLSLShader(std::string const &filename, std::string const &entry)
{
    return (new D3D12HLSLShader(filename, entry));
}

D3D12RenderableModel *D3D12RenderSystem::loadModel(GraphicPipeline &pipeline, std::string const &filename)
{
	D3D12RenderableModel *model = new D3D12RenderableModel;

	if (!model->loadObjFromFile(*this, dynamic_cast<D3D12GraphicPipeline&>(pipeline), filename))
	{
		std::cout << "Can't load model : " << filename << std::endl;

		delete model;
		return (nullptr);
	}

	return (model);
}