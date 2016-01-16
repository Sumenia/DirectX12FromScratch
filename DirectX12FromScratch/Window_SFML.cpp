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
}
