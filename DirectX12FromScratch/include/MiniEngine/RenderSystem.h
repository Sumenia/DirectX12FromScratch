#pragma once

# include <list>
# include "MiniEngine/HLSLShader.h"
# include "MiniEngine/CommandQueue.h"

namespace MiniEngine
{
    class RenderTarget;

    class RenderSystem
    {
    public:
        RenderSystem();
        virtual ~RenderSystem();

        virtual bool                init();
        virtual CommandQueue        *getCommandQueue() = 0;

        void                        addRenderTarget(RenderTarget *target);
        void                        clear();

        virtual HLSLShader          *createHLSLShader(std::string const &filename, std::string const &entry) = 0;

    protected:
        std::list<RenderTarget*>    _targets;
    };
}