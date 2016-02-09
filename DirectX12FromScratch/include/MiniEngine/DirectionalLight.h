#pragma once
#include "Light.h"

namespace MiniEngine
{
	class DirectionalLight : public Light
	{
	public:
        DirectionalLight(SceneManager &manager);
		DirectionalLight(SceneManager &manager, Vector3f direction);
		~DirectionalLight();

		void		setDirection(Vector3f direction);

		Light::Type	getType() const;

	private:
	};
}