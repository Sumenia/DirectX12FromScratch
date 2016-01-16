#pragma once

#include "IEvent.h"
#include <map>
#include <SFML/Window.hpp>

class Event_SFML : public IEvent
{
public:
	IEvent::EVENT_TYPE getEvent() const;
	Event_SFML();
private:
	std::map<sf::Keyboard::Key, IEvent::EVENT_TYPE> _map;
};