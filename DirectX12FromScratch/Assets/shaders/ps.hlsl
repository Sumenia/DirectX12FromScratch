#include "camera.hlsl"
#include "model.hlsl"
#include "psinput.hlsl"

float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color;
}
