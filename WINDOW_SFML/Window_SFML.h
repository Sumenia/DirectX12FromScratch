#pragma once

# include <map>
# include <SFML/Window.hpp>
# include "Window.h"
# include "Keyboard_SFML.h"

class Window_SFML : public Window
{
public:
    Window_SFML();
    virtual ~Window_SFML();

	virtual bool                create(unsigned int, unsigned int, HINSTANCE hInstance = nullptr);
    virtual void                *getHandle();
	virtual bool                isOpen() const;
	virtual void                destroy();

    virtual unsigned int        getWidth() const;
    virtual unsigned int        getHeight() const;

	// Inherited via IEvent
	virtual bool				getEvent(Event &event);

	virtual bool				isKeyPressed(Keyboard::Key key);

private:
	void						initEventTypeMap();

private:
	sf::Window											*_window;
	std::map<sf::Event::EventType, Event::EventType>	_eventTypeMap;
	Keyboard_SFML										_keyboard;
};