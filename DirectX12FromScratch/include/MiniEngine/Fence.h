#pragma once

namespace MiniEngine
{
    class RenderSystem;

    class Fence
    {
    public:
        Fence(RenderSystem &system);
        virtual ~Fence();

    protected:
        RenderSystem    &_system;
    };
}