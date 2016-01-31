#pragma once

#include "MiniEngine/RenderSystem.h"

namespace MiniEngine
{
	class ConstantBuffer
	{
	public:
		ConstantBuffer(RenderSystem&);
		virtual ~ConstantBuffer();

		virtual bool    init(unsigned int size) = 0;

        virtual void    update(CommandList &commandList, unsigned int size, void *data) = 0;
        virtual void    afterUpdate(CommandList &commandList) = 0;

	protected:
		RenderSystem	&_system;
	};
};