#include "Window_SFML.h"
#include "Config.h"

void Window_SFML::initEventTypeMap() {
	_eventTypeMap[sf::Event::Closed] = Event::Closed;
	_eventTypeMap[sf::Event::Resized] = Event::Resized;
	_eventTypeMap[sf::Event::LostFocus] = Event::LostFocus;
	_eventTypeMap[sf::Event::GainedFocus] = Event::GainedFocus;
	_eventTypeMap[sf::Event::TextEntered] = Event::TextEntered;
	_eventTypeMap[sf::Event::KeyPressed] = Event::KeyPressed;
	_eventTypeMap[sf::Event::KeyReleased] = Event::KeyReleased;
	_eventTypeMap[sf::Event::MouseWheelScrolled] = Event::MouseWheelScrolled;
	_eventTypeMap[sf::Event::MouseButtonPressed] = Event::MouseButtonPressed;
	_eventTypeMap[sf::Event::MouseButtonReleased] = Event::MouseButtonReleased;
	_eventTypeMap[sf::Event::MouseMoved] = Event::MouseMoved;
	_eventTypeMap[sf::Event::MouseEntered] = Event::MouseEntered;
	_eventTypeMap[sf::Event::MouseLeft] = Event::MouseLeft;
	_eventTypeMap[sf::Event::JoystickButtonPressed] = Event::JoystickButtonPressed;
	_eventTypeMap[sf::Event::JoystickButtonReleased] = Event::JoystickButtonReleased;
	_eventTypeMap[sf::Event::JoystickMoved] = Event::JoystickMoved;
	_eventTypeMap[sf::Event::JoystickConnected] = Event::JoystickConnected;
	_eventTypeMap[sf::Event::JoystickDisconnected] = Event::JoystickDisconnected;
	_eventTypeMap[sf::Event::TouchBegan] = Event::TouchBegan;
	_eventTypeMap[sf::Event::TouchMoved] = Event::TouchMoved;
	_eventTypeMap[sf::Event::TouchEnded] = Event::TouchEnded;
	_eventTypeMap[sf::Event::SensorChanged] = Event::SensorChanged;
}

Window_SFML::Window_SFML()
{
    _window = nullptr;
	initEventTypeMap();
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

bool Window_SFML::getEvent(Event &event)
{
	sf::Event sfmlEvent;

	if (_window->pollEvent(sfmlEvent))
	{
		event.type = _eventTypeMap[sfmlEvent.type];
		if (event.type == Event::KeyPressed || event.type == Event::KeyReleased) {
			event.key.code = _keyboard.fromNative(sfmlEvent.key.code);
			event.key.alt = sfmlEvent.key.alt;
			event.key.control = sfmlEvent.key.control;
			event.key.shift = sfmlEvent.key.shift;
			event.key.system = sfmlEvent.key.system;
		}
		else if (event.type == Event::TextEntered)
		{
			event.text.unicode = sfmlEvent.text.unicode;
		}
		else if (event.type == Event::MouseMoved)
		{
			event.mouseMove.x = sfmlEvent.mouseMove.x;
			event.mouseMove.y = sfmlEvent.mouseMove.y;
		}
		else if (event.type == Event::MouseButtonPressed || event.type == Event::MouseButtonReleased)
		{
			event.mouseButton.button = _mouse.fromNative(sfmlEvent.mouseButton.button);
			event.mouseButton.x = sfmlEvent.mouseButton.x;
			event.mouseButton.y = sfmlEvent.mouseButton.y;
		}
		else if (event.type == Event::MouseWheelScrolled)
		{
			event.mouseWheelScroll.wheel = _mouse.fromNative(sfmlEvent.mouseWheelScroll.wheel);
			event.mouseWheelScroll.delta = sfmlEvent.mouseWheelScroll.delta;
			event.mouseWheelScroll.x = sfmlEvent.mouseWheelScroll.x;
			event.mouseWheelScroll.y = sfmlEvent.mouseWheelScroll.y;
		}
		else if (event.type == Event::Resized)
		{
			event.size.width = sfmlEvent.size.width;
			event.size.height = sfmlEvent.size.height;
		}
		return (true);
	}

	return (false);
}

Keyboard *Window_SFML::getKeyboard()
{
	return &_keyboard;
}

Mouse *Window_SFML::getMouse()
{
	return &_mouse;
}
