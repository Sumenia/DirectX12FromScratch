#pragma once

# include "IWindow.h"
#include <SFML/Window.hpp>

class Window_SFML : public IWindow
{
public:
	virtual bool createWindow(unsigned int, unsigned int);

	virtual ~Window_SFML();

	virtual void *getHandle();

	virtual bool isOpen() const;

	Window_SFML();

private:
	sf::Window			*_window;
};