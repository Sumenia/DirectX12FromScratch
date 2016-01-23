#pragma once

# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
# endif

# include <windows.h>
# include "IWindow.h"

class Window_Win32 : public IWindow
{
public:
	Window_Win32();
	virtual ~Window_Win32();

	virtual bool                create(unsigned int heigth, unsigned int width, HINSTANCE hInstance);
	virtual void                *getHandle();
	virtual bool                isOpen() const;
	virtual void                destroy();

	virtual IWindow::EVENT_TYPE getEvent();

private:
	HWND        _handle;
	bool	    _isOpen;

	// callback function for windows messages
	//	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//
	//	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};