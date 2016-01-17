
#include <iostream>
#include "IWindow.h"
#include "DynamicLibrary.h"

#include "Graphics_DX12.h"


int	main()
{
	DynamicLibrary  dllClient;

	if (!dllClient.load("./" + DynamicLibrary::buildName("WINDOW_SFML")))
	{
		std::cerr << "Can't load dll client" << std::endl;
		return (EXIT_FAILURE);
	}
	std::function<IWindow*(void)> dll_window = (IWindow*(*)(void))dllClient.loadSymbol("entry");


	IWindow *win = dll_window();

	if (!win)
	{
		std::cerr << "Can't instantiation dll_window" << std::endl;
		return (EXIT_FAILURE);
	}

	if (!win->createWindow(800, 600))
	{
		std::cerr << "Can't create window" << std::endl;
		delete (win);
		return (EXIT_FAILURE);
	}

	IGraphics *grph = new Graphics_DX12();

	if (!grph->Init(800, 600, win->getHandle()))
	{
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
			win->close();
		}
	}

	grph->Quit();
	delete (grph);
	delete (win);

	return (EXIT_SUCCESS);
}