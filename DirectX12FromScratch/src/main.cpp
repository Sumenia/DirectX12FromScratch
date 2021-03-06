#include <iostream>
#include "MainApplication.h"
#include "MiniEngine\Mesh.h"

# if defined(SYSTEM_WINDOWS)
int WINAPI WinMain(HINSTANCE hInstance, 
	HINSTANCE,
	LPSTR,
	int)
# else
int	main()
# endif

;

int	main()
{
    MainApplication application("WINDOW_SFML");

	//MiniEngine::Mesh mesh;
	//mesh.loadObjFromFile("../../DirectX12FromScratch/DirectX12FromScratch/Assets/Cube.ntm");

    application.run();
    return (0);
}