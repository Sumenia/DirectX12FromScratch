struct Model
{
    float4x4 transform;
    float4x4 transformNormal;
};

ConstantBuffer<Model> model : register(b1);