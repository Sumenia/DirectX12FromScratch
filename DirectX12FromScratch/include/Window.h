#pragma once

#include <Windows.h>

class Window 
{
public:
	enum EVENT_TYPE
	{
		UP = 0,
		DOWN,
		RIGHT,
		LEFT,
		ESCAPE,
		UNDEFINED
	};

    virtual ~Window() {}

	virtual bool                create(unsigned int heigth, unsigned int width, HINSTANCE hInstance = nullptr) = 0;
	virtual	void                *getHandle() = 0;
	virtual	bool	            isOpen() const = 0;
	virtual void                destroy() = 0;
    
    virtual unsigned int        getWidth() const = 0;
    virtual unsigned int        getHeight() const = 0;

	virtual Window::EVENT_TYPE getEvent() = 0;

private:
};

