struct Camera
{
    float4x4 view;
    float4x4 projection;
};

ConstantBuffer<Camera> camera : register(b0);