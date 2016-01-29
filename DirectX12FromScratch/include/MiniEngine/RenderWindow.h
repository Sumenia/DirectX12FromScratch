#pragma once

# include "Window.h"
# include "MiniEngine/RenderTarget.h"

namespace MiniEngine
{
    class RenderWindow : virtual public RenderTarget
    {
    public:
        RenderWindow(RenderSystem &system, Window *window);
        ~RenderWindow();

        Viewport            *getDefaultViewport();

    protected:
        Window				*_window;
    };
}