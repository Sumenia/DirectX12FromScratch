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
    delete _device;
    delete _fence;
    _factory = nullptr;
}

bool D3D12RenderSystem::init()
{
    return (
        initDevice()
        && initCommandQueue()
        && initFence()
        && _commandQueue->wait(*_fence)
        && initDefaultMaterial()
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

    _device = D3D12Device::create(_factory);

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

bool D3D12RenderSystem::initDefaultMaterial()
{
    D3D12Material       *material = createMaterial();

    if (!material)
        return (false);

    if (!material->finalize())
    {
        delete material;
        return (false);
    }

    return (registerMaterial(material));
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

D3D12ConstantBuffer *D3D12RenderSystem::createConstantBuffer(unsigned int size)
{
    return (createConstantBuffer(size, D3D12RenderWindow::FrameCount));
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

D3D12HLSLShader *D3D12RenderSystem::createHLSLShader()
{
    return (new D3D12HLSLShader());
}

D3D12Material *D3D12RenderSystem::createMaterial()
{
    return (new D3D12Material(*this));
}

D3D12Texture *D3D12RenderSystem::createTexture(std::string const &filename)
{
    D3D12Texture    *texture = new D3D12Texture(*this);

    if (!texture->loadFromFile(filename))
    {
        delete texture;
        return (nullptr);
    }

    return (texture);
}

D3D12GraphicPipeline *D3D12RenderSystem::createGraphicPipeline(Material &material)
{
    HLSLShader              *vertexShader = nullptr;
    HLSLShader              *pixelShader = nullptr;

    D3D12GraphicPipeline    *pipeline = new D3D12GraphicPipeline(*this);
    
    // Set root signature (TO-DO: Generate it correctly with materials informations)
    {
        D3D12RootSignature      *rootSignature = new D3D12RootSignature();

        if (!rootSignature->init(*this, material.haveAmbientMap(), material.haveDiffuseMap(), material.haveSpecularMap()))
        {
            delete rootSignature;
            delete pipeline;

            return (nullptr);
        }

        pipeline->setRootSignature(rootSignature);
    }

    // Set input layouts
    {
        const HLSLShader::Input inputs[] = {
            { "POSITION", 0, MiniEngine::HLSLShader::Input::Format::R32G32B32_FLOAT, 0, 0, MiniEngine::HLSLShader::Input::Classification::PER_VERTEX, 0 },
            { "NORMAL", 0, MiniEngine::HLSLShader::Input::Format::R32G32B32_FLOAT, 0, 12, MiniEngine::HLSLShader::Input::Classification::PER_VERTEX, 0 },
			{ "TEXCOORD", 0, MiniEngine::HLSLShader::Input::Format::R32G32_FLOAT, 0, 24, MiniEngine::HLSLShader::Input::Classification::PER_VERTEX, 0 }
        };

        pipeline->setInputs(_countof(inputs), inputs);
    }

    // Generate vertex shader
    {
        vertexShader = createHLSLShader();

        if (!vertexShader->compile(MiniEngine::Shader::VERTEX, material.generateHLSLShader(Shader::VERTEX), "VSMain"))
        {
            std::cout << "Can't compile Vertex shader" << std::endl;

            delete vertexShader;
            delete pipeline;

            return (nullptr);
        }

        pipeline->addVertexShader(*vertexShader);
    }

    // Generate pixel shader
    {
        pixelShader = createHLSLShader();

        if (!pixelShader->compile(MiniEngine::Shader::PIXEL, material.generateHLSLShader(Shader::PIXEL), "PSMain"))
        {
            std::cout << "Can't compile Pixel shader" << std::endl;

            delete vertexShader;
            delete pixelShader;
            delete pipeline;

            return (nullptr);
        }

        pipeline->addPixelShader(*pixelShader);
    }

    if (!pipeline->finalize())
    {
        delete pipeline;
        delete vertexShader;
        delete pixelShader;

        return (nullptr);
    }

    delete vertexShader;
    delete pixelShader;

    return (pipeline);
}

D3D12RenderableModel *D3D12RenderSystem::loadModel(std::string const &filename)
{
	D3D12RenderableModel *model = new D3D12RenderableModel(*this);

	if (!model->loadFromFile(*this, filename))
	{
		std::cout << "Can't load model : " << filename << std::endl;

		delete model;
		return (nullptr);
	}

	return (model);
}