#pragma once
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
	virtual bool createWindow(unsigned int heigth, unsigned int width) = 0;
	virtual	void *getHandle() = 0;
	virtual	bool	isOpen() const = 0;
	virtual void close() = 0;
	virtual IWindow::EVENT_TYPE getEvent() = 0 ;
private:
};

