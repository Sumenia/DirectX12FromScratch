
#include "Window_SFML.h"
#include "DynamicLibrary.h"

extern "C"
{
	DLL_EXPORT IWindow* entry(void)
	{
		return (new Window_SFML());
	}
}