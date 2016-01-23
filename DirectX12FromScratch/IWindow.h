#pragma once

#include <Windows.h>

class IWindow 
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

    virtual ~IWindow() {}

	virtual bool                create(unsigned int heigth, unsigned int width, HINSTANCE hInstance = nullptr) = 0;
	virtual	void                *getHandle() = 0;
	virtual	bool	            isOpen() const = 0;
	virtual void                destroy() = 0;

	virtual IWindow::EVENT_TYPE getEvent() = 0;

private:
};

