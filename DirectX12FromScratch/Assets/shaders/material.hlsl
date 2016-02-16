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

SamplerState samp : register(s0);

#if TEXTURE_AMBIENT
Texture2D	ambientTexture : register(t0);
#endif

#if TEXTURE_DIFFUSE
Texture2D	diffuseTexture : register(t1);
#endif

#if TEXTURE_SPECULAR
Texture2D	specularTexture : register(t2);
#endif

float3 getDiffuseColor(float3 normal, float2 uv)
{
#if NORMAL_COLOR
    return (float3(abs(normal.x), abs(normal.y), abs(normal.z)));
#elif TEXTURE_DIFFUSE
    return (diffuseTexture.Sample(samp, uv).rgb);
#else
    return (material.kd);
#endif
}

float3 getAmbientColor(float3 normal, float2 uv)
{
#if TEXTURE_AMBIENT
    return (ambientTexture.Sample(samp, uv).rgb);
#else
    return (material.ka * getDiffuseColor(normal, uv));
#endif
}

float3 getSpecularColor(float3 normal, float2 uv)
{
#if TEXTURE_SPECULAR
    return (specularTexture.Sample(samp, uv).rgb);
#else
    return (material.ks * getDiffuseColor(normal, uv));
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