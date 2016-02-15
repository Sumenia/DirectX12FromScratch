#include "Window_SFML.h"
#include "Config.h"

Event::EventType Window_SFML::fromNative(sf::Event::EventType type) {
    switch (type)
    {
    case sf::Event::Closed:
        return Event::Closed;
    case sf::Event::Resized:
        return Event::Resized;
    case sf::Event::LostFocus:
        return Event::LostFocus;
    case sf::Event::GainedFocus:
        return Event::GainedFocus;
    case sf::Event::TextEntered:
        return Event::TextEntered;
    case sf::Event::KeyPressed:
        return Event::KeyPressed;
    case sf::Event::KeyReleased:
        return Event::KeyReleased;
    case sf::Event::MouseWheelScrolled:
        return Event::MouseWheelScrolled;
    case sf::Event::MouseButtonPressed:
        return Event::MouseButtonPressed;
    case sf::Event::MouseButtonReleased:
        return Event::MouseButtonReleased;
    case sf::Event::MouseMoved:
        return Event::MouseMoved;
    case sf::Event::MouseEntered:
        return Event::MouseEntered;
    case sf::Event::MouseLeft:
        return Event::MouseLeft;
    case sf::Event::JoystickButtonPressed:
        return Event::JoystickButtonPressed;
    case sf::Event::JoystickButtonReleased:
        return Event::JoystickButtonReleased;
    case sf::Event::JoystickMoved:
        return Event::JoystickMoved;
    case sf::Event::JoystickConnected:
        return Event::JoystickConnected;
    case sf::Event::JoystickDisconnected:
        return Event::JoystickDisconnected;
    case sf::Event::TouchBegan:
        return Event::TouchBegan;
    case sf::Event::TouchMoved:
        return Event::TouchMoved;
    case sf::Event::TouchEnded:
        return Event::TouchEnded;
    case sf::Event::SensorChanged:
        return Event::SensorChanged;
    default:
        return Event::Unknown;
    }
}

Window_SFML::Window_SFML()
{
    _window = nullptr;
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
        event.type = fromNative(sfmlEvent.type);
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

void Window_SFML::setMouseCursorVisible(bool visible)
{
	_window->setMouseCursorVisible(visible);
}

const sf::Window * Window_SFML::getSfmlWindow() const
{
	return _window;
}
