#include "MiniEngine/RenderWindow.h"

using namespace MiniEngine;

RenderWindow::RenderWindow(RenderSystem &system, Window *window) : RenderTarget(system), _window(window)
{
    addViewport(new Viewport({ 0, 0 }, { (float)window->getWidth(), (float)window->getHeight() }));
}

RenderWindow::~RenderWindow()
{}

Viewport *RenderWindow::getDefaultViewport()
{
    return (_viewports.front());
}