#pragma once

class IGraphics
{
public:
	virtual ~IGraphics() {}

	virtual bool Init(unsigned int width, unsigned int height, void* hwnd) = 0;
	virtual bool setOutputDisplay(void*) = 0;
	virtual void Quit() = 0;

	virtual bool Display() = 0;
};