#pragma once

#include "MiniEngine/RenderSystem.h"

namespace MiniEngine
{
	class ConstantBuffer
	{
	public:
		ConstantBuffer(RenderSystem&);
		~ConstantBuffer();

		virtual bool init() = 0;

	protected:
		RenderSystem	&_system;
	};
};