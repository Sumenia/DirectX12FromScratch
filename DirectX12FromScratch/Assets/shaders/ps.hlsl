#include "camera.hlsl"
#include "model.hlsl"
#include "psinput.hlsl"
#include "material.hlsl"
#include "lights.hlsl"

float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color;
}
