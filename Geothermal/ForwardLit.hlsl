#include "Common.hlsli"

#define ADDITIONAL_LIGHTS

Texture2D AlbedoMap : register(ALBEDO_MAP_SLOT);
Texture2D NormalMap : register(NORMAL_MAP_SLOT);
SamplerState Sampler;

cbuffer Properties : register(PROPERTIES_SLOT)
{
	float4		Ambient;
	float4		BaseColor;
	float		Diffuse;
	float		Specular;
	float		Smoothness;
	// In order to keep us aligned 
	// we combine UseAlbedoMap and UserNormalMap into the same field
	//	&0x01 bit for albedo, &0x02 bit for normal
	int			TextureFlags;
};

float3 ComputeWorldSpaceNormal(float3 pixelNormal, float3 pixelTangent, float3 normalSample) 
{
	normalSample = (2.0f * normalSample) - 1.0f;		// Bring sample range from [0,1] to [-1,1]

	pixelNormal = normalize(pixelNormal);
	// Apply Gram-Schmidt to make sure tangent is orthogonal to the normal
	pixelTangent = normalize(pixelTangent - dot(pixelTangent, pixelNormal) * pixelNormal);
	float3 biTangent = cross(pixelNormal, pixelTangent);
	float3x3 tangent2World = float3x3(pixelTangent, biTangent, pixelNormal);

	return normalize(mul(normalSample , tangent2World));
}

float4 main(Varyings input) : SV_TARGET
{
	float4 textureColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 normal = normalize(input.normal);

	// Sample textures if needed
	int useAlbedoMap = TextureFlags & 0x01;
	int useNormalMap = TextureFlags & 0x02;
	if (useAlbedoMap)
	{
		textureColor = AlbedoMap.Sample(Sampler, input.texcoord);
	}
	if (useNormalMap)
	{
		float3 normalSample = NormalMap.Sample(Sampler, input.texcoord).xyz;
		// Compute normal
		// by transporting normal sample from tangent space to world space
		normal = ComputeWorldSpaceNormal(input.normal, input.tangent, normalSample);
	}

	// Lighting
	float4 pixelColor = BaseColor + textureColor;
	// We want light direction to point TOWARDS the light, thus we negate main light direction
	float3 mainLightDirection = normalize(-MainLight.Direction);
	float intensity = 0.0f;
	//intensity = BlinnPhong(normal, input.worldPosition, mainLightDirection, Diffuse, Specular, Smoothness);

#ifdef	ADDITIONAL_LIGHTS
	// For each additional light
	for (uint i = 0; i < LightsActivation.x; i++)
	{
		Light light = AdditionalLights[i];
		float3 lightDirection = light.Position - input.worldPosition;
		if (length(lightDirection) < light.Radius)
		{
			intensity += BlinnPhong(normal, input.worldPosition, normalize(lightDirection), Diffuse, Specular, Smoothness);
		}
	}
#endif

	pixelColor.xyz *= intensity;
	pixelColor += Ambient;

	return pixelColor;
}