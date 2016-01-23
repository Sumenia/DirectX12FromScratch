#include <iostream>
#include "Application.h"

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
    // Render one frame with the render system
    return (true);
}