#include "psinput.hlsl"
#include "camera.hlsl"
#include "material.hlsl"
#include "lights.hlsl"

float3 computePointLight(float3 materialColor, Light light, PSInput input)
{
    float3  normal = normalize(input.normal);
    float3  position = float3(input.worldPosition.x, input.worldPosition.y, input.worldPosition.z);

    // Compute ambient color
    float3  ambient = light.ambient * materialColor; // TO-DO: Use ka

    // Compute diffuse color
    float3  lightDirection = normalize(light.position - position);
    float   diff = max(dot(normal, lightDirection), 0.0f);
    float3  diffuse = light.diffuse * diff * materialColor; // TO-DO: Use kd

    // Compute specular color
    float3  viewDir = normalize(camera.position - position);
    float3  reflectionDirection = reflect(-lightDirection, normal);
    float   spec = pow(max(dot(viewDir, reflectionDirection), 0.0), 32.0f/* TO-DO: Replace by shininess */);
    float3  specular = light.specular * spec * materialColor; // TO-DO: Use ks

    // TO-DO: Compute attenuation

    return (ambient + diffuse + specular);
}

float3 computeLight(float3 materialColor, Light light, PSInput input)
{
    // TO-DO: Manage others types
    if (light.type == 0) // POINT
        return (computePointLight(materialColor, light, input));
    else
        return (float3(0.0f, 0.0f, 0.0f));
}

float4 PSMain(PSInput input) : SV_TARGET
{
    float3  materialColor = getMaterialColor(input.normal);
    float3  color;

    for (uint i = 0; i < camera.nb_lights; i++)
        color += computeLight(materialColor, lights[i], input);

    return (float4(color, 1.0f));
}
