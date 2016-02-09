#pragma once
#include "Light.h"

namespace MiniEngine
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight(SceneManager &manager, float angle);
		~DirectionalLight();

		float	getAngle() const;
		void	setAngle();
	private:
		float	_angle;
	};
}