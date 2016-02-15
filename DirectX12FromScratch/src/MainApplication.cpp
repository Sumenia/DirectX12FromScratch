#include <iostream>
#include "MainApplication.h"
#include "MiniEngine/SpotLight.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12RenderWindow.h"
#include "MiniEngine/Texture.h"

MainApplication::MainApplication(const std::string &windowType, HINSTANCE hInstance) : MiniEngine::Application(), _window(nullptr)
{
    float       clearColor[4] = { 0.2f, 0.2f, 0.2f, 0.0f };

    initWindow(windowType, hInstance);

    if (_window)
    {
		MiniEngine::Texture text;

        _root->setRenderSystem(new MiniEngine::D3D12RenderSystem);

        if (_root->getRenderSystem())
        {
            MiniEngine::RenderWindow        *renderTarget = _root->getRenderSystem()->createRenderWindow(_window);

            // Add render target
            _root->getRenderSystem()->addRenderTarget(renderTarget);

            // Change clear color
            renderTarget->setClearColor(clearColor);

            // Create Scene
            _sceneManager = _root->createSceneManager(MiniEngine::Root::SceneManagerType::BASIC);

            if (_sceneManager)
            {
                _camera = _sceneManager->createCamera();
                _camera->lookAt({ 10.0f, 10.0f, 20.0f }, { 0.0f, -0.1f, 0.0f }, { 0.0f, 1.0f, 0.0f });

                renderTarget->getDefaultViewport()->attachCamera(_camera);

                std::shared_ptr<MiniEngine::SpotLight>          light = std::dynamic_pointer_cast<MiniEngine::SpotLight>(_sceneManager->createLight(MiniEngine::Light::SPOT, _camera));

                light->setAmbient({ 0.1f, 0.1f, 0.1f });
                light->setDiffuse({ 0.5f, 0.5f, 0.5f });
                light->setSpecular({ 1.0f, 1.0f, 1.0f });

                light->setInnerCutOff(10.0f);
                light->setOuterCutOff(25.0f);
                
                light->setDirection({ 0.0f, 0.0f, -1.0f });

				MiniEngine::SceneNode                           *node;
				std::shared_ptr<MiniEngine::RenderableModel>    model = _root->getRenderSystem()->loadModel("./Assets/models/majora/Majora.txt");

				for (unsigned int i = 0; i < 400; i++)
				{
					node = _sceneManager->getRootNode()->createChild(model);
					node->scale({ 25.0f, 25.0f, 25.0f });
					node->translate({ i % 40 * 50.0f, i / 40 * 50.0f, 0.0f });
				}
            }
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

#include "MiniEngine/Clock.h"

bool MainApplication::update(MiniEngine::Time elapsedTime)
{
    static MiniEngine::Clock    clock;

    if (!_window || !_window->isOpen())
        return (false);

    float elapsedSeconds = elapsedTime.getSeconds();

    std::cout << (1.0f / clock.reset().getSeconds()) << " FPS" << std::endl;

	if (_window->isKeyPressed(Keyboard::Left))
		_camera->rotate(100 * elapsedSeconds, Vector3f(0, 1, 0), MiniEngine::TS_WORLD);
	if (_window->isKeyPressed(Keyboard::Right))
		_camera->rotate(100 * elapsedSeconds, Vector3f(0, -1, 0), MiniEngine::TS_WORLD);
	if (_window->isKeyPressed(Keyboard::Up))
		_camera->rotate(100 * elapsedSeconds, Vector3f(1, 0, 0), MiniEngine::TS_LOCAL);
	if (_window->isKeyPressed(Keyboard::Down))
		_camera->rotate(100 * elapsedSeconds, Vector3f(-1, 0, 0), MiniEngine::TS_LOCAL);

	if (_window->isKeyPressed(Keyboard::Z))
		_camera->translate(Vector3f(0, 0, -100 * elapsedSeconds), MiniEngine::TS_LOCAL);
	if (_window->isKeyPressed(Keyboard::S))
		_camera->translate(Vector3f(0, 0, 100 * elapsedSeconds), MiniEngine::TS_LOCAL);
	if (_window->isKeyPressed(Keyboard::Q))
		_camera->translate(Vector3f(-100 * elapsedSeconds, 0, 0), MiniEngine::TS_LOCAL);
	if (_window->isKeyPressed(Keyboard::D))
		_camera->translate(Vector3f(100 * elapsedSeconds, 0, 0), MiniEngine::TS_LOCAL);

	if (_window->isKeyPressed(Keyboard::Space))
		_camera->translate(Vector3f(0, 100 * elapsedSeconds, 0), MiniEngine::TS_WORLD);
	if (_window->isKeyPressed(Keyboard::LShift))
		_camera->translate(Vector3f(0, -100 * elapsedSeconds, 0), MiniEngine::TS_WORLD);

	Event event;

	while (_window->getEvent(event))
	{
		if (event.type == Event::KeyPressed)
		{
			if (event.key.code == Keyboard::Escape)
			{
				_window->destroy();
				return (false);
			}
		}

		if (event.type == Event::Closed)
		{
			_window->destroy();
			return (false);
		}
	}

    return (true);
}