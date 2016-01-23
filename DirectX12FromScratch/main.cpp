
#include <iostream>
#include "IWindow.h"
#include "DynamicLibrary.h"

#include "Graphics_DX12.h"

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
		std::cerr << "Can't load dll client" << std::endl;
		return (EXIT_FAILURE);
	}

	std::function<IWindow*(void)> dll_window = (IWindow*(*)(void))dllClient.loadSymbol("entry");

	IWindow *win = dll_window();

	if (!win)
	{
		OutputDebugStringW(L"Can't instantiation dll_window.\n");

		std::cerr << "Can't instantiation dll_window" << std::endl;
		return (EXIT_FAILURE);
	}

# if defined(SYSTEM_WINDOWS)
	if (!win->create(800, 600, hInstance))
# else 
	if (!win->create(800, 600))
# endif
	{
		std::cerr << "Can't create window" << std::endl;
		
        delete (win);
		return (EXIT_FAILURE);
	}

	IGraphics *grph = new Graphics_DX12();

	if (!grph->Init(800, 600, win->getHandle()))
	{
		std::cerr << "Can't init DX12" << std::endl;

		delete (win);
		return (EXIT_FAILURE);
	}

	while (win->isOpen())
	{
		grph->Display();

		if (win->getEvent() == IWindow::EVENT_TYPE::ESCAPE)
		    win->destroy();
	}

	grph->Quit();

	delete (grph);
	delete (win);

	return (EXIT_SUCCESS);
}