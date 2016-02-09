#include "psinput.hlsl"
#include "camera.hlsl"
#include "material.hlsl"
#include "lights.hlsl"

float smoothStep(float min, float max, float x)
{
    if (x < min)
    {
        return (0.0f);
    }
    else if (x >= max)
    {
        return (1.0f);
    }
    else
    {
        float value = (x - min) / (max - min);
        float  pow1 = value * value;
        float pow2 = pow1 * value;
        return ((-2.0f * pow2) + (3.0f * pow1));
    }
}

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
    float3  reflectionDirection = reflect(lightDirection, normal);
    float   spec = pow(max(dot(viewDir, reflectionDirection), 0.0), 32.0f/* TO-DO: Replace by shininess */);
    float3  specular = light.specular * spec * materialColor; // TO-DO: Use ks

    // Compute attenuation
    float	distance = length(light.position - position);
    float	attenuationFactor = 1 / (light.constantAttenuation + light.linearAttenuation * distance + light.quadraticAttenuation * distance * distance);

    return (attenuationFactor * (ambient + diffuse + specular));
}

float computeDirectionalLight(float3 materialColor, Light light, PSInput input)
{
	float3  diffuse = float3(0, 0, 0);
	float3 specular = float3(0, 0, 0);
	float3  position = float3(input.worldPosition.x, input.worldPosition.y, input.worldPosition.z);

	//The light vector aims opposite the direction the light rays travel.
	float3 lightVector = -1 * (light.position - position);

	// Add ambient term.
	float3  ambient = light.ambient * materialColor;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light
	float3  normal = normalize(input.normal);

	float diffuseFactor = dot(lightVector, normal);
	if (diffuseFactor > 0.0f) {
		float3 vect = reflect(-lightVector, normal);
		//pow(max(dot(viewDir, reflectionDirection), 0.0), 32.0f/ TO-DO: Replace by shininess /);
		float specularFactor = pow(max(dot(vect, position), 0.0f), 32.0f);

		diffuse = light.diffuse * diffuseFactor * materialColor;
		specular = light.specular * specularFactor * materialColor;
	}
	return (diffuse + specular);
}

float3 computeSpotLigth(float3 materialColor, Light light, PSInput input)
{
	float3  position = float3(input.worldPosition.x, input.worldPosition.y, input.worldPosition.z);

	float3  lightDirection = normalize(light.direction);//light.position - position;

	float3  V = normalize(position - light.position);

	float cosDirection = dot(V, lightDirection);

	float cosInnerCutOff = cos(light.cutOff);

	float cosOuterCutOff = cos(light.outerCutOff);

	float spotEffect = smoothStep(light.outerCutOff, light.cutOff, cosDirection);

	float3  normal = normalize(input.normal);
	
	return  computePointLight(materialColor, light, input) * spotEffect;
}

float3 computeLight(float3 materialColor, Light light, PSInput input)
{
    // TO-DO: Manage others types
    if (light.type == 0) // POINT
        return (computePointLight(materialColor, light, input));
    else if (light.type == 1) // DIRECTIONAL
        return (computeDirectionalLight(materialColor, light, input));
    else if (light.type == 2) // SPOT
        return (computeSpotLigth(materialColor, light, input));
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
