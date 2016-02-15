struct Material
{
    uint    id;

#if !TEXTURE_AMBIENT
    float3  ka;
#endif

#if !NORMAL_COLOR && !TEXTURE_DIFFUSE 
    float3  kd;
#endif

#if !TEXTURE_SPECULAR
    float3  ks;
#endif

    float   shininess;
};

ConstantBuffer<Material> material : register(b3);

float3 getDiffuseColor(float3 normal)
{
#if NORMAL_COLOR
    return (float3(abs(normal.x), abs(normal.y), abs(normal.z)));
#elif TEXTURE_DIFFUSE
    // TO-DO: RETURN TEXTURE
#else
    return (material.kd);
#endif
}

float3 getNormal(float3 normal)
{
#if NORMAL_MAP
    // TO-DO: Use normal map
#else
    return (normal);
#endif
}