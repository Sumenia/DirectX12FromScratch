#include <iostream>
#include "MainApplication.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12RenderWindow.h"

MainApplication::MainApplication(const std::string &windowType, HINSTANCE hInstance) : MiniEngine::Application(), _window(nullptr)
{
    initWindow(windowType, hInstance);

    if (_window)
    {
        _root->setRenderSystem(new MiniEngine::D3D12RenderSystem);

        if (_root->getRenderSystem())
        {
            MiniEngine::RenderTarget        *renderTarget = _root->getRenderSystem()->createRenderWindow(_window);
            MiniEngine::GraphicPipeline     *pipeline;

            MiniEngine::HLSLShader          *vertexShader = _root->getRenderSystem()->createHLSLShader("./Assets/shaders.hlsl", "VSMain");
            MiniEngine::HLSLShader          *pixelShader = _root->getRenderSystem()->createHLSLShader("./Assets/shaders.hlsl", "PSMain");

            // Add render target
            _root->getRenderSystem()->addRenderTarget(renderTarget);

            // Compile shader
            if (!vertexShader->compile(MiniEngine::Shader::VERTEX))
                std::cout << "Can't compile Vertex shader" << std::endl;

            if (!pixelShader->compile(MiniEngine::Shader::PIXEL))
                std::cout << "Can't compile Pixel shader" << std::endl;

            // Create pipeline
            pipeline = renderTarget->getGraphicPipeline();
            pipeline->addVertexShader(*vertexShader);
            pipeline->addPixelShader(*pixelShader);
            pipeline->finalize();

            delete vertexShader;
            delete pixelShader;
        }
    }
}

MainApplication::~MainApplication()
{}

void MainApplication::initWindow(const std::string &windowType, HINSTANCE hInstance)
{
    if (!_dllWindow.load("./" + DynamicLibrary::buildName(windowType.c_str())))
    {
        std::cerr << "Can't load dll : " << DynamicLibrary::buildName(windowType.c_str()) << std::endl;
        return;
    }

    std::function<Window*(void)> createWindow = (Window*(*)(void))_dllWindow.loadSymbol("entry");

    if (!createWindow)
    {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
        std::cerr << "Can't load dll entry point" << std::endl;
        return;
    }

    _window = createWindow();

    if (!_window)
    {
        std::cerr << "Can't create instance of Window" << std::endl;
        return;
    }

# if defined(SYSTEM_WINDOWS)
    if (!_window->create(800, 600, hInstance))
# else 
    if (!_window->create(800, 600))
# endif
    {
        std::cerr << "Can't create window" << std::endl;

        delete (_window);
        _window = nullptr;
    }
}

bool MainApplication::update()
{
    if (!_window || !_window->isOpen())
        return (false);

    if (_window->getEvent() == Window::EVENT_TYPE::ESCAPE)
    {
        _window->destroy();
        return (false);
    }

    return (true);
}