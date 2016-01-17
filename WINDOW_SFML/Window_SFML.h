#pragma once

# include <map>
# include <SFML/Window.hpp>
# include "IWindow.h"

class Window_SFML : public IWindow
{
public:
	virtual bool createWindow(unsigned int, unsigned int, HINSTANCE hInstance);

	virtual ~Window_SFML();

	virtual void *getHandle();

	virtual bool isOpen() const;

	Window_SFML();

	virtual void close();

	// Inherited via IEvent
	virtual IWindow::EVENT_TYPE getEvent();


private:
	sf::Window			*_window;
	std::map<sf::Keyboard::Key, IWindow::EVENT_TYPE> _map;

};