cbuffer CameraConstantBuffer : register(b0)
{
    matrix view;
    matrix projection;
};

cbuffer ModelConstantBuffer : register(b1)
{
    matrix model;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PSInput VSMain(float3 position : POSITION, float3 normal : NORMAL)
{
	PSInput result;

    float4 worldPosition = float4(position, 1.0f);

    worldPosition = mul(worldPosition, model);
    worldPosition = mul(worldPosition, view);
    worldPosition = mul(worldPosition, projection);

	result.position = worldPosition;
	result.color = float4(abs(normal.x), abs(normal.y), abs(normal.z), 1.0f);

	return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	return input.color;
}
