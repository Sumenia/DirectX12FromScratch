#pragma once

# include <map>
# include <SFML/Window.hpp>
# include "IWindow.h"

class Window_SFML : public IWindow
{
public:
    Window_SFML();
    virtual ~Window_SFML();

	virtual bool                create(unsigned int, unsigned int, HINSTANCE hInstance = nullptr);
    virtual void                *getHandle();
	virtual bool                isOpen() const;
	virtual void                destroy();

	// Inherited via IEvent
	virtual IWindow::EVENT_TYPE getEvent();

private:
	sf::Window			                                *_window;
	std::map<sf::Keyboard::Key, IWindow::EVENT_TYPE>    _map;

};