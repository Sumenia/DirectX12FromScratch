struct Camera
{
    float3      position;
    float4x4    view;
    float4x4    projection;
    uint        nb_lights;
};

ConstantBuffer<Camera> camera : register(b0);