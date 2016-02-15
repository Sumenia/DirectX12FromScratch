#pragma once
#include "MiniEngine/Light.h"

namespace MiniEngine
{
	class SpotLight : public Light
	{
	public:
        SpotLight(SceneManager &manager);
		SpotLight(SceneManager &manager, Vector3f direction, float innerCutOffAngle, float outterCutOffAngle);
		SpotLight(SceneManager &manager, Vector3f direction, float innerCutOffAngle);

		~SpotLight();

		void		setOuterCutOff(float outerCutOff);
		void		setInnerCutOff(float cutOff);
		void		setDirection(Vector3f direction);
        void	    setRange(float range);

		Light::Type getType() const;

	private:
	};
}