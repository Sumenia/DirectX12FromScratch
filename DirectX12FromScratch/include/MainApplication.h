#pragma once

# include <string>
# include "Window.h"
# include "MiniEngine/Application.h"
# include "DynamicLibrary.h"

class MainApplication : public MiniEngine::Application
{
public:
    MainApplication(const std::string &windowType = "WINDOW_SFML", HINSTANCE hInstance = nullptr);
    ~MainApplication();

    void            initWindow(const std::string &windowType, HINSTANCE hInstance);

    bool            update(MiniEngine::Time elapsedTime);

protected:
	Window          *_window;
	Keyboard		*_keyboard;
    DynamicLibrary  _dllWindow;

	MiniEngine::SceneManager	*_sceneManager;
	MiniEngine::Camera			*_camera;
};