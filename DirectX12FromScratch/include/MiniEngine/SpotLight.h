#pragma once
#include "Light.h"

namespace MiniEngine
{
	class SpotLight : public Light
	{
	public:
		SpotLight(SceneManager &manager, Vector3f direction, float innerCutOffAngle, float outterCutOffAngle);
		~SpotLight();

		float		getOuterCutOff() const;
		void		setOuterCutOff(float outerCutOff);

		float		getInnerCutOff() const;
		void		setInnerCutOff(float cutOff);

		Vector3f	getDirection() const;
		void		setDirection(Vector3f direction);

	private:
		float           _outerCutOffAngle;
		float           _innerCutOffAngle;
		Vector3f        _direction;
	};
}