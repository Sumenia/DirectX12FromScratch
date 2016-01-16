#pragma once
class IWindow
{
public:
	virtual ~IWindow() {}
	virtual bool createWindow(unsigned int heigth, unsigned int width) = 0;
	virtual	void *getHandle() = 0;
	virtual	bool	isOpen() const = 0;

private:
};

