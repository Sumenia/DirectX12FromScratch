#include <iostream>
#include "MainApplication.h"
#include "MiniEngine\Mesh.h"

# include "ImageLoader/JpgImageLoader.h"

# if defined(SYSTEM_WINDOWS)
int WINAPI WinMain(HINSTANCE hInstance, 
	HINSTANCE,
	LPSTR,
	int)
# else
int	main()
# endif

;

int	main(int ac, char **av)
{
    MainApplication application("WINDOW_SFML");

	JpgImageLoader	test;

	test.loadFromFile("../../DirectX12FromScratch/DirectX12FromScratch/test_jpeg.jpg");


	//MiniEngine::Mesh mesh;
	//mesh.loadObjFromFile("../../DirectX12FromScratch/DirectX12FromScratch/Assets/Cube.ntm");

    application.run();
    return (0);
}