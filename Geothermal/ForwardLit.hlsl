#include "Common.hlsli"

Texture2D AlbedoMap : register(ALBEDO_MAP_SLOT);
Texture2D NormalMap : register(NORMAL_MAP_SLOT);
SamplerState Sampler;

cbuffer Properties : register(PROPERTIES_SLOT)
{
	MaterialProperty property;
};

float4 main(Varyings input) : SV_TARGET
{
	float4 textureColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 normal = normalize(input.normal);

	// Parse texture flags
	int useAlbedoMap = property.TextureFlags & 0x01;
	int useNormalMap = property.TextureFlags & 0x02;
	int useShadowMap = property.TextureFlags & 0x04;

	// Sample textures if needed
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
	float4 pixelColor = property.BaseColor + textureColor;
	float intensity = 0.0f;
	if (useShadowMap)
	{
		// Evaluate IsInShadow iff we use shadow map
		if (!IsInShadow(input.lightSpacePosition, Sampler))
		{
			intensity = 
				BlinnPhong
				(
					normal, input.worldPosition, -MainLight.Direction, 
					property.Diffuse, property.Specular, property.Smoothness
				);
		}
	}
	else
	{
		intensity = 
			BlinnPhong
			(
				normal, input.worldPosition, -MainLight.Direction, 
				property.Diffuse, property.Specular, property.Smoothness
			);
	}

	for (uint i = 0; i < LightsActivation.x; i++)
	{
		float3 pointToLight = AdditionalLights[i].Position.xyz - input.worldPosition;
		if (length(pointToLight) < AdditionalLights[i].Radius)
		{
			float3 lightDirection = normalize(pointToLight);
			intensity += 
				BlinnPhong
				(
					normal, input.worldPosition, lightDirection, 
					property.Diffuse, property.Specular, property.Smoothness
				);
		}
	}

	pixelColor.xyz *= intensity;
	pixelColor += property.Ambient;

	return pixelColor;
}
