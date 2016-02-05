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
            DIRECTIONAL = 0,
            POINT = 1,
            SPOT = 2,
        };

    public:
        Light(SceneManager &manager);
        virtual ~Light();

        virtual void    needUpdate();

    protected:
        SceneManager    &_manager;

        Type            _type;
        Vector3f        _direction;
        float           _cutOff;
        float           _outerCutOff;

        Vector3f        _ambient;
        Vector3f        _diffuse;
        Vector3f        _specular;

        float           _constant;
        float           _linear;
        float           _quadratic;
    };
}