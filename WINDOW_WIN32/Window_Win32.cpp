#include "Window_Win32.h"

Window_Win32::Window_Win32()
{
	_handle = nullptr;
	_isOpen = false;
}

Window_Win32::~Window_Win32()
{
}

bool Window_Win32::create(unsigned int height, unsigned int width, HINSTANCE hInstance)
{
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = Window_Win32::WndProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	//	wc.hInstance = hInstan;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = "MainMenu";
	wc.lpszClassName = "MainWClass";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	OutputDebugStringW(L"Init WNDCLASSEX OK.\n");

	if (!RegisterClassEx(&wc))
	{
		OutputDebugStringW(L"CANT REGISTER.\n");

		return false;
	}
	_handle = CreateWindowEx(NULL,
		"MainWClass",
		"Sample",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height,
		NULL,
		NULL,
		hInstance,
		//		hInstance,
		NULL);
	OutputDebugStringW(L"CA FONCTIONNE !!.\n");

	//_handle = CreateWindowEx(NULL,
	//	"DXSampleName",
	//	"DXSampleTitle",
	//	WS_OVERLAPPEDWINDOW,
	//	CW_USEDEFAULT, CW_USEDEFAULT,
	//	width, height,
	//	NULL,
	//	NULL,
	//	hInstance,
	//	//		hInstance,
	//	NULL);

	if (!_handle)
	{
		OutputDebugStringW(L"HANDLE APRES CREATE WINDOW EST NULL.\n");

		return false;
	}
	_isOpen = true;
	ShowWindow(_handle, 1);
	UpdateWindow(_handle);
	return (true);
}

void * Window_Win32::getHandle()
{
	if (_handle)
		return (_handle);
	return nullptr;
}

bool Window_Win32::isOpen() const
{
	return (_isOpen);
}

void Window_Win32::destroy()
{
	if (_handle)
		DestroyWindow(_handle);
}

unsigned int Window_Win32::getWidth() const
{
    RECT rc;

    GetClientRect(_handle, &rc);
    return (rc.right);
}

unsigned int Window_Win32::getHeight() const
{
    RECT rc;

    GetClientRect(_handle, &rc);
    return (rc.bottom);
}

bool Window_Win32::getEvent(Event &event)
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			_isOpen = false;
			event.type = Event::Closed;
			return (true);
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return false;
}

LRESULT Window_Win32::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd,
		msg,
		wParam,
		lParam);


	return LRESULT();
}
