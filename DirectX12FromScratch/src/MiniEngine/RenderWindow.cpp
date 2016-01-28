#include "MiniEngine/RenderWindow.h"

using namespace MiniEngine;

RenderWindow::RenderWindow(RenderSystem &system, Window *window) : RenderTarget(system), _window(window)
{}

RenderWindow::~RenderWindow()
{}