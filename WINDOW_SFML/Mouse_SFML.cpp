#include "Mouse_SFML.h"
#include "Window_SFML.h"

Mouse_SFML::Mouse_SFML()
{
	_fromNative[sf::Mouse::Left] = Mouse::Left;
	_fromNative[sf::Mouse::Right] = Mouse::Right;
	_fromNative[sf::Mouse::Middle] = Mouse::Middle;
	_fromNative[sf::Mouse::XButton1] = Mouse::XButton1;
	_fromNative[sf::Mouse::XButton2] = Mouse::XButton2;


	_toNative[Mouse::Left] = sf::Mouse::Left;
	_toNative[Mouse::Right] = sf::Mouse::Right;
	_toNative[Mouse::Middle] = sf::Mouse::Middle;
	_toNative[Mouse::XButton1] = sf::Mouse::XButton1;
	_toNative[Mouse::XButton2] = sf::Mouse::XButton2;
}

sf::Mouse::Button Mouse_SFML::toNative(Mouse::Button key)
{
	return _toNative[key];
}

Mouse::Button Mouse_SFML::fromNative(sf::Mouse::Button key)
{
	return _fromNative[key];
}

sf::Mouse::Wheel Mouse_SFML::toNative(Mouse::Wheel key)
{
	if (key == Mouse::HorizontalWheel)
		return sf::Mouse::HorizontalWheel;
	return sf::Mouse::VerticalWheel;
}

Mouse::Wheel Mouse_SFML::fromNative(sf::Mouse::Wheel key)
{
	if (key == sf::Mouse::HorizontalWheel)
		return Mouse::HorizontalWheel;
	return Mouse::VerticalWheel;
}

bool Mouse_SFML::isButtonPressed(Mouse::Button button)
{
	return sf::Mouse::isButtonPressed(toNative(button));
}

Vector2i Mouse_SFML::getPosition()
{
	sf::Vector2i v = sf::Mouse::getPosition();
	return Vector2i(v.x, v.y);
}

Vector2i Mouse_SFML::getPosition(const Window & relativeTo)
{
	Window_SFML const *window = nullptr;

	if (!(window = dynamic_cast<const Window_SFML*>(&relativeTo)))
	{
		return Vector2i();
	}
	sf::Vector2i v = sf::Mouse::getPosition(*window->getSfmlWindow());
	return Vector2i(v.x, v.y);
}

void Mouse_SFML::setPosition(const Vector2i & position)
{
	sf::Mouse::setPosition(sf::Vector2i(position.x, position.y));
}

void Mouse_SFML::setPosition(const Vector2i & position, const Window & relativeTo)
{
	Window_SFML const *window = nullptr;

	if (!(window = dynamic_cast<const Window_SFML*>(&relativeTo)))
	{
		return;
	}
	sf::Mouse::setPosition(sf::Vector2i(position.x, position.y), *window->getSfmlWindow());
}
