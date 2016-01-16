#include "Window_SFML.h"

bool Window_SFML::createWindow(unsigned int h, unsigned int w)
{
	_window = new sf::Window(sf::VideoMode(h, w), "My window");
	if (_window)
		return (true);
	return (false);
}

Window_SFML::~Window_SFML()
{
	delete (_window);
}

void * Window_SFML::getHandle()
{
	if (_window)
		return _window->getSystemHandle();
	return (nullptr);
}

bool Window_SFML::isOpen() const
{
	if (_window)
		return (_window->isOpen());
	return (false);
}

Window_SFML::Window_SFML()
{
	_window = nullptr;
	_map[sf::Keyboard::Escape] = ESCAPE;
}

void Window_SFML::close()
{
	_window->close();
}

IWindow::EVENT_TYPE Window_SFML::getEvent()// const
{
	sf::Event event;

	if (_window->pollEvent(event))
	{
		return (_map.find(event.key.code) != _map.end() ? _map[event.key.code] : IWindow::EVENT_TYPE::UNDEFINED);
	}
	return IWindow::EVENT_TYPE::UNDEFINED;
}