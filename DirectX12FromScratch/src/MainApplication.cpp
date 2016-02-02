#include <iostream>
#include "MainApplication.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12RenderWindow.h"

MainApplication::MainApplication(const std::string &windowType, HINSTANCE hInstance) : MiniEngine::Application(), _window(nullptr)
{
    float       clearColor[4] = { 0.2f, 0.2f, 0.2f, 0.0f };

    initWindow(windowType, hInstance);

    if (_window)
    {
        _root->setRenderSystem(new MiniEngine::D3D12RenderSystem);

        if (_root->getRenderSystem())
        {
            MiniEngine::RenderWindow        *renderTarget = _root->getRenderSystem()->createRenderWindow(_window);
            MiniEngine::GraphicPipeline     *pipeline;

            MiniEngine::HLSLShader          *vertexShader = _root->getRenderSystem()->createHLSLShader("./Assets/shaders.hlsl", "VSMain");
            MiniEngine::HLSLShader          *pixelShader = _root->getRenderSystem()->createHLSLShader("./Assets/shaders.hlsl", "PSMain");

            // Add render target
            _root->getRenderSystem()->addRenderTarget(renderTarget);

            // Change clear color
            renderTarget->setClearColor(clearColor);

            // Compile shader
            if (!vertexShader->compile(MiniEngine::Shader::VERTEX))
                std::cout << "Can't compile Vertex shader" << std::endl;

            if (!pixelShader->compile(MiniEngine::Shader::PIXEL))
                std::cout << "Can't compile Pixel shader" << std::endl;

            // Create pipeline
            pipeline = renderTarget->getGraphicPipeline();

            const MiniEngine::HLSLShader::Input    inputs[] = {
                { "POSITION", 0, MiniEngine::HLSLShader::Input::Format::R32G32B32_FLOAT, 0, 0, MiniEngine::HLSLShader::Input::Classification::PER_VERTEX, 0 },
                { "COLOR", 0, MiniEngine::HLSLShader::Input::Format::R32G32B32_FLOAT, 0, 12, MiniEngine::HLSLShader::Input::Classification::PER_VERTEX, 0 }
            };

            pipeline->setInputs(2, inputs);

            pipeline->addVertexShader(*vertexShader);
            pipeline->addPixelShader(*pixelShader);

            pipeline->finalize();

            // Create Scene
            _sceneManager = _root->createSceneManager(MiniEngine::Root::SceneManagerType::BASIC);
            _camera = _sceneManager->createCamera();

            _camera->lookAt({ 0.0f, 50.7f, 1.5f }, { 0.0f, -0.1f, 0.0f }, { 0.0f, 1.0f, 0.0f });

            renderTarget->getDefaultViewport()->attachCamera(_camera);

            // Load a cube
			_node = _sceneManager->getRootNode()->createChild(_root->getRenderSystem()->loadMesh(*pipeline, "./Assets/ship.txt"));

            //_node->rotate(45, MiniEngine::Vector3f(1.0f, 0.0f, 0.0f));
            //_node->scale(MiniEngine::Vector3f(1.0f, 0.5f, 0.5f));

            delete vertexShader;
            delete pixelShader;
        }
    }
}

MainApplication::~MainApplication()
{
    delete _sceneManager;
}

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

	Window::EVENT_TYPE event = _window->getEvent();

	if (event == Window::EVENT_TYPE::LEFT)
	{
		_node->rotate(1, MiniEngine::Vector3f(0, -1, 0), MiniEngine::TS_PARENT);

	}
	else if (event == Window::EVENT_TYPE::RIGHT)
	{
        _node->rotate(1, MiniEngine::Vector3f(0, 1, 0), MiniEngine::TS_PARENT);

	}
	else if (event == Window::EVENT_TYPE::UP)
	{
        _node->rotate(1, MiniEngine::Vector3f(-1, 0, 0), MiniEngine::TS_PARENT);
	}
	else if (event == Window::EVENT_TYPE::DOWN)
	{
        _node->rotate(1, MiniEngine::Vector3f(1, 0, 0), MiniEngine::TS_PARENT);
	}

	else if (event == Window::EVENT_TYPE::ESCAPE)
    {
        _window->destroy();
        return (false);
    }

    return (true);
}