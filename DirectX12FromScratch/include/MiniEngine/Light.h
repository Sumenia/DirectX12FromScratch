#pragma once

#include "MiniEngine/MovableObject.h"
#include "MiniEngine/Geometry.h"

# define MAX_LIGHTS 16

namespace MiniEngine
{
    class SceneManager;

    class Light : public MovableObject
    {
    public:
        enum Type : unsigned int
        {
            POINT = 0,
            DIRECTIONAL = 1,
            SPOT = 2,
        };

    public:
        Light(SceneManager &manager);
        virtual ~Light();

        virtual void    needUpdate();

        virtual Type    getType() const = 0;

        Vector3f        &getAmbient();
        void            setAmbient(Vector3f const &ambient);

        Vector3f        &getDiffuse();
        void            setDiffuse(Vector3f const &diffuse);

        Vector3f        &getSpecular();
        void            setSpecular(Vector3f const &specular);

		void			setConstant(float constant);
		float			getConstant();

		void			setLinear(float linear);
		float			getLinear();

		void			setQuadratic(float quadratic);
		float			getQuadratic();

		/* DirectionalLight */
		Vector3f		getDirection() const;

		/* SpotLight */
		float			getOuterCutOff() const;
		float			getInnerCutOff() const;
		float			getCosineBigAngle() const;
		float			getCosineLightAngle() const;

		/* PointLight + Spotlight */
		float			getRange() const;

    protected:
        SceneManager    &_manager;

		Vector3f        _ambient;
        Vector3f        _diffuse;
        Vector3f        _specular;

        float           _constant;
        float           _linear;
        float           _quadratic;

		/* DirectionalLight + SpotLight */
		Vector3f		_direction;

		/* SpotLight */
		float           _outerCutOffAngle;
		float           _innerCutOffAngle;

		/* PointLight + Spotlight */
		float			_range;
    };
}