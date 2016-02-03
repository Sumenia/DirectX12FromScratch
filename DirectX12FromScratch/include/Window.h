#pragma once

#include <Windows.h>
#include "Event.h"

class Window 
{
public:
    virtual ~Window() {}

	virtual bool                create(unsigned int heigth, unsigned int width, HINSTANCE hInstance = nullptr) = 0;
	virtual	void                *getHandle() = 0;
	virtual	bool	            isOpen() const = 0;
	virtual void                destroy() = 0;
    
    virtual unsigned int        getWidth() const = 0;
    virtual unsigned int        getHeight() const = 0;

	virtual bool				getEvent(Event &) = 0;

private:
};

