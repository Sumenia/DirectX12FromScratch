#pragma once
#include "Light.h"

namespace MiniEngine
{
	class PointLight : public Light
	{
	public:
		PointLight(SceneManager &manager, float intansity);
		~PointLight();

		void	setIntansity(float intensity);
		float	getIntansity() const;
	private:
		float		_intancity;
	};
}