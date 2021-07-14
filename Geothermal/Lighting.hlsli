#ifndef GEOTHERMAL_LIGHTING
#define GEOTHERMAL_LIGHTING

float Lambert(float3 normal, float3 lightDirection, float diffuseStrength)
{
	float diffuse = max(
		0.0f,
		dot(normal, lightDirection)
	) * diffuseStrength;
	return diffuse;
}

float BlinnPhong(float3 normal, float3 worldPosition, float3 lightDirection, float diffuseStrength, float specularStrength, float smoothness)
{
	float3 viewDirection = normalize(CameraWorldPosition - worldPosition);
	float3 halfway = normalize(lightDirection + viewDirection);

	float diffuse = Lambert(normal, lightDirection, diffuseStrength);

	float specular = pow(
		max(0.0f, dot(normal, halfway)), smoothness
	) * specularStrength;

	return diffuse + specular;
}

struct DirectionalLight
{
	float4 Color;
	float3 Direction;
	float	Padding0;
};

struct Light
{
	float4 Color;
	float4	Position;
	float	Radius;
	float	Attenuation;
	int		Type;
	float	Padding0;
};

#define MAX_POINT_LIGHTS_IN_SCENE 32

// Common lighting info
cbuffer Lights : register(LIGHTS_SLOT)
{
	DirectionalLight	MainLight;
	Light						AdditionalLights[MAX_POINT_LIGHTS_IN_SCENE];
	// The x-component specify how many additional lights are active
	float4						LightsActivation;
};

#endif