#include "psinput.hlsl"
#include "camera.hlsl"
#include "material.hlsl"
#include "lights.hlsl"

float3 calcAmbientColor(Light light, float2 uv, float3 normal)
{
    return (light.ambient * getAmbientColor(normal, uv));
}

float3 calcDiffuseColor(Light light, float2 uv, float3 normal, float3 lightDirection)
{
    float   diff = max(dot(normal, lightDirection), 0.0f);

    return (light.diffuse * diff * getDiffuseColor(normal, uv));
}

float3 calcSpecularColor(Light light, float2 uv, float3 position, float3 normal, float3 lightDirection)
{
    float3  viewDir = normalize(camera.position - position);
    float3  halfWayDir = -normalize(-lightDirection + viewDir);
    float   spec = pow(max(dot(normal, halfWayDir), 0.0f), material.shininess);

    return (light.specular * spec * getSpecularColor(normal, uv));
}

float3 calcAttenuate(Light light, float3 distance, float3 ambient, float3 diffuse, float3 specular)
{
    float	attenuationFactor = 1 / (light.constantAttenuation + light.linearAttenuation * distance + light.quadraticAttenuation * distance * distance);

    return (attenuationFactor * (ambient + diffuse + specular));
}

float3 computePointLight(Light light, float3 position, float3 normal, float2 uv)
{
    float3  lightDirection = normalize(position - light.position);
    float	distance = length(light.position - position);

    if (light.range >= 0.0f && distance > light.range)
        return (float3(0.0f, 0.0f, 0.0f));

    // Compute different colors
    float3  ambient = calcAmbientColor(light, uv, normal);
    float3  diffuse = calcDiffuseColor(light, uv, normal, lightDirection);
    float3  specular = calcSpecularColor(light, uv, position, normal, lightDirection);

    return (calcAttenuate(light, distance, ambient, diffuse, specular));
}

float3 computeDirectionalLight(Light light, float3 position, float3 normal, float2 uv)
{
    float3  lightDirection = normalize(light.direction);

    // Compute different colors
    float3  ambient = calcAmbientColor(light, uv, normal);
    float3  diffuse = calcDiffuseColor(light, uv, normal, lightDirection);
    float3  specular = calcSpecularColor(light, uv, position, normal, lightDirection);

    return (ambient + diffuse + specular);
}

float3 computeSpotLigth(Light light, float3 position, float3 normal, float2 uv)
{
    float3  lightDirection = normalize(position - light.position);
    float	distance = length(light.position - position);

    if (light.range >= 0.0f && distance > light.range)
        return (float3(0.0f, 0.0f, 0.0f));

    // Compute different colors
    float3  ambient = calcAmbientColor(light, uv, normal);
    float3  diffuse = calcDiffuseColor(light, uv, normal, lightDirection);
    float3  specular = calcSpecularColor(light, uv, position, normal, lightDirection);

    // Soft edges
    float   theta = dot(lightDirection, normalize(-light.direction));
    float   epsilon = light.cutOff - light.outerCutOff;
    float   intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

    diffuse *= intensity;
    specular *= intensity;

    return (calcAttenuate(light, distance, ambient, diffuse, specular));
}

float3 computeLight(Light light, float3 position, float3 normal, float2 uv)
{
    if (light.type == 0) // POINT
        return (computePointLight(light, position, normal, uv));
    else if (light.type == 1) // DIRECTIONAL
        return (computeDirectionalLight(light, position, normal, uv));
    else if (light.type == 2) // SPOT
        return (computeSpotLigth(light, position, normal, uv));
    else
        return (float3(0.0f, 0.0f, 0.0f));
}

float4 PSMain(PSInput input) : SV_TARGET
{
    float3  normal = normalize(input.normal);
    float3  position = float3(input.worldPosition.x, input.worldPosition.y, input.worldPosition.z);

	float3  color = float3(0.0f, 0.0f, 0.0f);

	for (uint i = 0; i < camera.nb_lights; i++)
		color += computeLight(lights[i], position, normal, input.uv);

    return (float4(color, 1.0f));
}