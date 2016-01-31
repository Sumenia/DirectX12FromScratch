#pragma once

# include <list>
# include "Window.h"
# include "MiniEngine/HLSLShader.h"
# include "MiniEngine/CommandQueue.h"

namespace MiniEngine
{
    class RenderTarget;
    class RenderWindow;
    class RenderableMesh;

    class RenderSystem
    {
    public:
        RenderSystem();
        virtual ~RenderSystem();

        virtual bool                init();
        virtual CommandQueue        *getCommandQueue() = 0;

        void                        addRenderTarget(RenderTarget *target);
        void                        clear();

        virtual RenderWindow        *createRenderWindow(Window *window) = 0;
        virtual HLSLShader          *createHLSLShader(std::string const &filename, std::string const &entry) = 0;

        virtual RenderableMesh      *loadMesh(GraphicPipeline &pipeline, std::string const &filename) = 0;

        bool                        render();

    protected:
        std::list<RenderTarget*>    _targets;
    };
}