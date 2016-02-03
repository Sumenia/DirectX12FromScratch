#pragma once

#include <map>
#include <SFML/Window/Keyboard.hpp>
#include "Keyboard.h"

class Keyboard_SFML : Keyboard
{
public:
	Keyboard_SFML();

	bool				isKeyPressed(Keyboard::Key keycode);
	sf::Keyboard::Key	toNative(Keyboard::Key);
	Keyboard::Key		fromNative(sf::Keyboard::Key);

private:
	std::map<sf::Keyboard::Key, Keyboard::Key>			_fromNative;
	std::map<Keyboard::Key, sf::Keyboard::Key>			_toNative;
};