#pragma once

# include "MiniEngine/Root.h"
# include "MiniEngine/Time.h"

namespace MiniEngine
{
    class Application
    {
    public:
        Application();
        virtual ~Application();

        void            run();
        virtual bool    update(Time elapsedTime) = 0;

    protected:
        bool            renderOneFrame();

        Root            *_root;
    };
}