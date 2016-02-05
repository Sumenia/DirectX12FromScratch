struct Material
{
    uint    id;

#if UNIFORM_COLOR 
    float3  color;
#endif
};

ConstantBuffer<Material> material : register(b3);

float3 getMaterialColor(float3 normal)
{
#if NORMAL_COLOR
    return (float3(abs(normal.x), abs(normal.y), abs(normal.z)));
#elif UNIFORM_COLOR
    return (material.color);
#elif TEXTURE
    // TO-DO: RETURN TEXTURE
#endif
}

float3 getMaterialNormal(float3 normal)
{
#if NORMAL_MAP
    // TO-DO: Use normal map
#else
    return (normal);
#endif
}