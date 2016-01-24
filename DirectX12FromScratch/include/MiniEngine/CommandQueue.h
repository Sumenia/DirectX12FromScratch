#pragma once

# include "MiniEngine/Fence.h"

namespace MiniEngine
{
    class CommandQueue
    {
    public:
        CommandQueue();
        virtual ~CommandQueue();

        virtual bool    wait(Fence &fence) = 0;
    };
}