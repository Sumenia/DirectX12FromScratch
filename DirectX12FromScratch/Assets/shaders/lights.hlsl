# define MAX_LIGHT  16

struct Light
{
    uint    type;

    float3  position;
    float3  direction;

    float   cutOff;
    float   outerCutOff;

    float3  ambient;
    float3  diffuse;
    float3  specular;

    float   constantAttenuation;
    float   linearAttenuation;
    float   quadraticAttenuation;
};

cbuffer lightsArray : register(b2)
{
    Light   lights[MAX_LIGHT];
}