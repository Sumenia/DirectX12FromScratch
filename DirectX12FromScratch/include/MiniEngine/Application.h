#pragma once

# include "Root.h"

namespace MiniEngine
{
    class Application
    {
    public:
        Application();
        virtual ~Application();

        void            run();
        virtual bool    update() = 0;

    protected:
        bool            renderOneFrame();

        Root            *_root;
    };
}