struct Camera
{
    float4x4 view;
    float4x4 projection;
};

ConstantBuffer<Camera> camera : register(b0);

struct Model
{
    float4x4 transform;
    float4x4 transformNormal;
};

ConstantBuffer<Model> model : register(b1);

struct PSInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PSInput VSMain(float3 position : POSITION, float3 normal : NORMAL)
{
	PSInput result;

    normal = mul(float4(normal, 0), model.transformNormal);

    float4 worldPosition = float4(position, 1.0f);

    worldPosition = mul(worldPosition, model.transform);
    worldPosition = mul(worldPosition, camera.view);
    worldPosition = mul(worldPosition, camera.projection);

	result.position = worldPosition;
	result.color = float4(abs(normal.x), abs(normal.y), abs(normal.z), 1.0f);

	return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	return input.color;
}
