#pragma once

# include <map>
# include <SFML/Window.hpp>
# include "Window.h"

class Window_SFML : public Window
{
public:
    Window_SFML();
    virtual ~Window_SFML();

	virtual bool                create(unsigned int, unsigned int, HINSTANCE hInstance = nullptr);
    virtual void                *getHandle();
	virtual bool                isOpen() const;
	virtual void                destroy();

	// Inherited via IEvent
	virtual Window::EVENT_TYPE getEvent();

private:
	sf::Window			                                *_window;
	std::map<sf::Keyboard::Key, Window::EVENT_TYPE>    _map;

};