#include "Window_SFML.h"
#include "Config.h"

Window_SFML::Window_SFML()
{
    _window = nullptr;
	_map[sf::Keyboard::Left] = LEFT;
	_map[sf::Keyboard::Right] = RIGHT;
	_map[sf::Keyboard::Up] = UP;
	_map[sf::Keyboard::Down] = DOWN;
	_map[sf::Keyboard::Escape] = ESCAPE;
	_map[sf::Keyboard::I] = ZOOM_IN;
	_map[sf::Keyboard::O] = ZOOM_OUT;
}

Window_SFML::~Window_SFML()
{
	if (_window && _window->isOpen())
		_window->close();

	delete (_window);
}

bool Window_SFML::create(unsigned int h, unsigned int w, HINSTANCE)
{
    _window = new sf::Window(sf::VideoMode(h, w), "My window");

    if (_window)
        return (true);

    return (false);
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

void Window_SFML::destroy()
{
	_window->close();
}

unsigned int Window_SFML::getWidth() const
{
    return (_window->getSize().x);
}

unsigned int Window_SFML::getHeight() const
{
    return (_window->getSize().y);
}

Window::EVENT_TYPE Window_SFML::getEvent()
{
	sf::Event event;

	if (_window->pollEvent(event))
	{
		return (_map.find(event.key.code) != _map.end() ? _map.at(event.key.code) : Window::EVENT_TYPE::UNDEFINED);
	}

	return Window::EVENT_TYPE::UNDEFINED;
}