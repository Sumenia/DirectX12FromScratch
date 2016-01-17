
#include <iostream>
#include "IWindow.h"
#include "DynamicLibrary.h"

#include "Graphics_DX12.h"

//#include "Window_Win32.h"

# if defined(SYSTEM_WINDOWS)
int WINAPI WinMain(HINSTANCE hInstance, 
	HINSTANCE,
	LPSTR,
	int)
# else
int	main()
# endif
{

	DynamicLibrary  dllClient;
//	if (!dllClient.load("./" + DynamicLibrary::buildName("WINDOW_SFML")))
	if (!dllClient.load("./" + DynamicLibrary::buildName("WINDOW_WIN32")))
	{
		OutputDebugStringW(L"CANT LOAD DLL.\n");

		std::cerr << "Can't load dll client" << std::endl;
		return (EXIT_FAILURE);
	}
	std::function<IWindow*(void)> dll_window = (IWindow*(*)(void))dllClient.loadSymbol("entry");

	std::cout << "je suis la" << std::endl;
	OutputDebugStringW(L"JE SUIS LA.\n");

	IWindow *win = dll_window();
//	IWindow *win = new Window_Win32();


	if (!win)
	{
		OutputDebugStringW(L"Can't instantiation dll_window.\n");

		std::cerr << "Can't instantiation dll_window" << std::endl;
		return (EXIT_FAILURE);
	}

# if defined(SYSTEM_WINDOWS)
	if (!win->createWindow(800, 600, hInstance))//, hInstance))
# else 
	if (!win->createWindow(800, 600))
# endif
	{
		std::cerr << "Can't create window" << std::endl;
		OutputDebugStringW(L"Can't create window.\n");

		delete (win);
		return (EXIT_FAILURE);
	}

	IGraphics *grph = new Graphics_DX12();

	if (!grph->Init(800, 600, win->getHandle()))
	{
		OutputDebugStringW(L"Can't init DX12.\n");
		std::cerr << "Can't init DX12" << std::endl;
		win->close();
		delete (win);
		return (EXIT_FAILURE);
	}

	while (win->isOpen())
	{
		grph->Display();

		if (win->getEvent() == IWindow::EVENT_TYPE::ESCAPE)
		{
			OutputDebugStringW(L"recu ECHAP.\n");

			win->close();
		}
	}
	OutputDebugStringW(L"Je quite.\n");

	grph->Quit();
	delete (grph);
	delete (win);

	return (EXIT_SUCCESS);
}