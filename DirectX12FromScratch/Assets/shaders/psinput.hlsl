struct PSInput
{
    float4  worldPosition : WORLD_POSITION;
    float4  position : SV_POSITION;
    float3  normal : NORMAL;
	float2	uv : TEXCOORD;
};
