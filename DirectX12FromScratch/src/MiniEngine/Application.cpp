#include <iostream>
#include "MiniEngine/Application.h"
#include "MiniEngine/Clock.h"

using namespace MiniEngine;

Application::Application()
{
    _root = new Root;
}

Application::~Application()
{
    delete _root;
}

void Application::run()
{
	Clock clock;
	int interval = 1000000 / 60;
    while (true)
    {
		if (clock.getElapsedTime().getMicroseconds() > interval) {
			if (!update(clock.getElapsedTime()))
				return;

			if (!renderOneFrame())
				return;
		
			clock.reset();
		}
    }
}

bool Application::renderOneFrame()
{
    if (_root->getRenderSystem())
        return (_root->getRenderSystem()->render());

    return (false);
}