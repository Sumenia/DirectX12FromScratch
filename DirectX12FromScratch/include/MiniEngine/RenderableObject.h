#pragma once

namespace MiniEngine
{
    class Camera;
    class CommandList;

    class RenderableObject
    {
    public:
        virtual bool        render(Camera &camera, CommandList &commandList) = 0;
    };
}