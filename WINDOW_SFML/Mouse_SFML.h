#pragma once

#include <unordered_map>
#include <SFML/Window/Mouse.hpp>
#include "Mouse.h"

class Mouse_SFML : public Mouse
{
public:
	Mouse_SFML();

	bool				isButtonPressed(Mouse::Button button);
	sf::Mouse::Button	toNative(Mouse::Button);
	Mouse::Button		fromNative(sf::Mouse::Button);

	sf::Mouse::Wheel	toNative(Mouse::Wheel);
	Mouse::Wheel		fromNative(sf::Mouse::Wheel);

	Vector2i			getPosition();
	Vector2i			getPosition(const Window& relativeTo);
	void				setPosition(const Vector2i& position);
	void				setPosition(const Vector2i &position, const Window &relativeTo);

private:
	std::unordered_map<sf::Mouse::Button, Mouse::Button>	_fromNative;
	std::unordered_map<Mouse::Button, sf::Mouse::Button>	_toNative;
};