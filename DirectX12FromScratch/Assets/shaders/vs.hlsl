#include "camera.hlsl"
#include "model.hlsl"
#include "psinput.hlsl"
#include "material.hlsl"

PSInput VSMain(float3 position : POSITION, float3 normal : NORMAL)
{
	PSInput result;

    normal = mul(float4(normal, 0), model.transformNormal);

    float4 worldPosition = float4(position, 1.0f);

    worldPosition = mul(worldPosition, model.transform);
    worldPosition = mul(worldPosition, camera.view);
    worldPosition = mul(worldPosition, camera.projection);

	result.position = worldPosition;
    result.color = float4(getMaterialColor(normal), 1.0f);

	return result;
}