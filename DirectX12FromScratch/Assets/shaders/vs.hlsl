#include "camera.hlsl"
#include "model.hlsl"
#include "psinput.hlsl"
#include "material.hlsl"

PSInput VSMain(float3 position : POSITION, float3 normal : NORMAL)
{
	PSInput result;

    float4 worldPosition = float4(position, 1.0f);

    worldPosition = mul(worldPosition, model.transform);
    result.worldPosition = worldPosition;

    float4 cameraPosition = mul(worldPosition, camera.view);
    cameraPosition = mul(cameraPosition, camera.projection);

	result.position = cameraPosition;
    result.normal = mul(float4(getMaterialNormal(normal), 0), model.transformNormal);

	return result;
}