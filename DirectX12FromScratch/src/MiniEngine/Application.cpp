#include <iostream>
#include "MiniEngine/Application.h"

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
    while (true)
    {
		if (!update())
            return;

        if (!renderOneFrame())
            return;
    }
}

bool Application::renderOneFrame()
{
    if (_root->getRenderSystem())
        return (_root->getRenderSystem()->render());

    return (false);
}