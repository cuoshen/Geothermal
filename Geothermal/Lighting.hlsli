#ifndef GEOTHERMAL_LIGHTING
#define GEOTHERMAL_LIGHTING

#include "SlotConfig.hlsli"

struct MaterialProperty
{
	float4		ambient;
	float4		baseColor;
	float		diffuse;
	float		specular;
	float		smoothness;
	// In order to keep us aligned , we combine texture usage into a single int32
	//	&0x01 for albedo, &0x02 for normal, &0x03 for shadow
	int			textureFlags;
};

float Lambert(float3 normal, float3 lightDirection, float diffuseStrength)
{
	float diffuse = max(
		0.0f,
		dot(normal, lightDirection)
	) * diffuseStrength;
	return diffuse;
}

float BlinnPhong
(
	float3 normal, float3 worldPosition, float3 lightDirection, float diffuseStrength,
	float specularStrength, float smoothness, float3 cameraWorldPosition
)
{
	float3 viewDirection = normalize(cameraWorldPosition - worldPosition);
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
	float	Intensity;
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

#endif