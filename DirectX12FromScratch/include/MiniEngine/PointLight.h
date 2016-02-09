#pragma once
#include "Light.h"

namespace MiniEngine
{
	class PointLight : public Light
	{
	public:
		PointLight(SceneManager &manager, float range);
		~PointLight();

		void	setRange(float range);
	};
}