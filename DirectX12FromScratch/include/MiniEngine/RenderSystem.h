#pragma once

# include <list>
# include <map>
# include "Window.h"
# include "MiniEngine/HLSLShader.h"
# include "MiniEngine/CommandQueue.h"

namespace MiniEngine
{
    class RenderTarget;
    class RenderWindow;
	class RenderableModel;
	class ConstantBuffer;
    class Material;

    class RenderSystem
    {
    public:
        RenderSystem();
        virtual ~RenderSystem();

        virtual bool                        init();
        virtual CommandQueue                *getCommandQueue() = 0;

        void                                addRenderTarget(RenderTarget *target);
        void                                clear();

        virtual ConstantBuffer              *createConstantBuffer(unsigned int size, unsigned int nb) = 0;
        virtual ConstantBuffer              *createConstantBuffer(unsigned int size) = 0;
        virtual RenderWindow                *createRenderWindow(Window *window) = 0;
        virtual HLSLShader                  *createHLSLShader() = 0;
        virtual Material                    *createMaterial() = 0;
        virtual GraphicPipeline             *createGraphicPipeline(Material &material) = 0;

		virtual RenderableModel             *loadModel(std::string const &filename) = 0;

        virtual Material                    *getMaterial(unsigned int id);
        virtual void                        deleteMaterial(unsigned int id);
        virtual bool                        registerMaterial(Material *material);
        virtual GraphicPipeline             *getGraphicPipeline(DWORD64 type);

        bool                                render();
		const std::list<RenderTarget *>		&getRenderTargets() const;

    protected:
        std::list<RenderTarget*>            _targets;

        std::map<DWORD64, GraphicPipeline*> _pipelines;
        std::map<unsigned int, Material*>   _materials;
    };
}