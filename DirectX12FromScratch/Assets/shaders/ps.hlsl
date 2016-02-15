#include "psinput.hlsl"
#include "camera.hlsl"
#include "material.hlsl"
#include "lights.hlsl"

float3 calcAmbientColor(Light light, float3 materialColor)
{
    return (light.ambient * materialColor); // TO-DO: Use ka
}

float3 calcDiffuseColor(Light light, float3 materialColor, float3 normal, float3 lightDirection)
{
    float   diff = max(dot(normal, lightDirection), 0.0f);

    return (light.diffuse * diff * materialColor); // TO-DO: Use kd
}

float3 calcSpecularColor(Light light, float3 materialColor, float3 position, float3 normal, float3 lightDirection)
{
    float3  viewDir = normalize(camera.position - position);
    float3  halfWayDir = -normalize(-lightDirection + viewDir);
    float   spec = pow(max(dot(normal, halfWayDir), 0.0f), 32.0f); // TO-DO: Replace by shininess

    return (light.specular * spec * materialColor); // TO-DO: Use ks
}

float3 calcAttenuate(Light light, float3 distance, float3 ambient, float3 diffuse, float3 specular)
{
    float	attenuationFactor = 1 / (light.constantAttenuation + light.linearAttenuation * distance + light.quadraticAttenuation * distance * distance);

    return (attenuationFactor * (ambient + diffuse + specular));
}

float3 computePointLight(float3 materialColor, Light light, float3 position, float3 normal)
{
    float3  lightDirection = normalize(position - light.position);
    float	distance = length(light.position - position);

    if (light.range >= 0.0f && distance > light.range)
        return (float3(0.0f, 0.0f, 0.0f));

    // Compute different colors
    float3  ambient = calcAmbientColor(light, materialColor);
    float3  diffuse = calcDiffuseColor(light, materialColor, normal, lightDirection);
    float3  specular = calcSpecularColor(light, materialColor, position, normal, lightDirection);

    return (calcAttenuate(light, distance, ambient, diffuse, specular));
}

float3 computeDirectionalLight(float3 materialColor, Light light, float3 position, float3 normal)
{
    float3  lightDirection = normalize(light.direction);

    // Compute different colors
    float3  ambient = calcAmbientColor(light, materialColor);
    float3  diffuse = calcDiffuseColor(light, materialColor, normal, lightDirection);
    float3  specular = calcSpecularColor(light, materialColor, position, normal, lightDirection);

    return (ambient + diffuse + specular);
}

float3 computeSpotLigth(float3 materialColor, Light light, float3 position, float3 normal)
{
    float3  lightDirection = normalize(position - light.position);
    float	distance = length(light.position - position);

    if (light.range >= 0.0f && distance > light.range)
        return (float3(0.0f, 0.0f, 0.0f));

    // Compute different colors
    float3  ambient = calcAmbientColor(light, materialColor);
    float3  diffuse = calcDiffuseColor(light, materialColor, normal, lightDirection);
    float3  specular = calcSpecularColor(light, materialColor, position, normal, lightDirection);

    // Soft edges
    float   theta = dot(lightDirection, normalize(-light.direction));
    float   epsilon = light.cutOff - light.outerCutOff;
    float   intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

    diffuse *= intensity;
    specular *= intensity;

    return (calcAttenuate(light, distance, ambient, diffuse, specular));
}

float3 computeLight(float3 materialColor, Light light, float3 position, float3 normal)
{
    if (light.type == 0) // POINT
        return (computePointLight(materialColor, light, position, normal));
    else if (light.type == 1) // DIRECTIONAL
        return (computeDirectionalLight(materialColor, light, position, normal));
    else if (light.type == 2) // SPOT
        return (computeSpotLigth(materialColor, light, position, normal));
    else
        return (float3(0.0f, 0.0f, 0.0f));
}

/*float4 PSMain(PSInput input) : SV_TARGET
{
    float3  normal = normalize(input.normal);
    float3  position = float3(input.worldPosition.x, input.worldPosition.y, input.worldPosition.z);

	float3  materialColor = getMaterialColor(input.normal);
	float3  color;

	for (uint i = 0; i < camera.nb_lights; i++)
		color += computeLight(materialColor, lights[i], position, normal);

    return (float4(color, 1.0f));
}*/

Texture2D	tex : register(t0);

SamplerState samp : register(s0);

float4 PSMain(PSInput input) : SV_TARGET
{
	return (tex.Sample(samp, input.uv));
	//return (float4(1.0, 1.0, 1.0, 1.0));
}
