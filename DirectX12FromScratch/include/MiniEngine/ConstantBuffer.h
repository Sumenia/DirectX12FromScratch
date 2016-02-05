#pragma once

#include "MiniEngine/RenderSystem.h"

namespace MiniEngine
{
    class Light;

	class ConstantBuffer
	{
	public:
		ConstantBuffer(RenderSystem&);
		virtual ~ConstantBuffer();

		virtual bool    init(unsigned int size, unsigned int nb) = 0;

        virtual bool    update(unsigned int size, void *data) = 0;
        virtual bool    bind(CommandList &commandList, unsigned int rootIdx) = 0;

        virtual bool    updateCameraMatrix(Matrix4f const &view, Matrix4f const &projection, unsigned int nb_lights) = 0;
        virtual bool    updateModelMatrix(Matrix4f const &model) = 0;
        virtual bool    updateLights(std::list<Light*> &lights) = 0;

	protected:
		RenderSystem	&_system;
	};
};