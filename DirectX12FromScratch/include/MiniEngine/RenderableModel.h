#pragma once

# include "MiniEngine/Model.h"
# include "MiniEngine/RenderableObject.h"
# include "MiniEngine/MovableObject.h"

namespace MiniEngine
{
	class RenderableModel : public Model, public RenderableObject, public MovableObject
	{
	public:
		RenderableModel();
		virtual ~RenderableModel();
	};
}