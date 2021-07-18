#include "Common.hlsli"

Texture2D AlbedoMap : register(ALBEDO_MAP_SLOT);
Texture2D NormalMap : register(NORMAL_MAP_SLOT);
Texture2D ShadowMap : register(t2);
SamplerState Sampler;

cbuffer Properties : register(PROPERTIES_SLOT)
{
	float4		Ambient;
	float4		BaseColor;
	float		Diffuse;
	float		Specular;
	float		Smoothness;
	// In order to keep us aligned 
	// we combine texture usage into a single int32
	//	&0x01 for albedo, &0x02 for normal, &0x03 for shadow
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

#define SHADOW_BIAS 0.0001f

bool IsInShadow(float4 lightSpacePosition)
{
	// Project into Light Space NDC
	float2 projectedPosition;
	projectedPosition.x = (lightSpacePosition.x / lightSpacePosition.w / 2.0f) + 0.5f;
	projectedPosition.y = (-lightSpacePosition.y / lightSpacePosition.w / 2.0f) + 0.5f;

	// Check whether both x and y falls within (0,1), if so it may fall in shadow
	if 
	(
		(saturate(projectedPosition.x) == projectedPosition.x) &&
		(saturate(projectedPosition.y) == projectedPosition.y)
	)
	{
		float depthReachedByLight = ShadowMap.Sample(Sampler, projectedPosition).r;
		depthReachedByLight += SHADOW_BIAS;
		float depthOfPoint = lightSpacePosition.z / lightSpacePosition.w;

		return (depthOfPoint > depthReachedByLight);
	}

	return false;		// Unaffected by this light source
}

float4 main(Varyings input) : SV_TARGET
{
	float4 textureColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 normal = normalize(input.normal);

	// Parse texture flags
	int useAlbedoMap = TextureFlags & 0x01;
	int useNormalMap = TextureFlags & 0x02;
	int useShadowMap = TextureFlags & 0x04;

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
	float4 pixelColor = BaseColor + textureColor;
	float intensity = 0.0f;
	if (useShadowMap)
	{
		// Evaluate IsInShadow iff we use shadow map
		if (!IsInShadow(input.lightSpacePosition))
		{
			intensity = BlinnPhong(normal, input.worldPosition, -MainLight.Direction, Diffuse, Specular, Smoothness);
		}
	}
	else
	{
		intensity = BlinnPhong(normal, input.worldPosition, -MainLight.Direction, Diffuse, Specular, Smoothness);
	}

	for (uint i = 0; i < LightsActivation.x; i++)
	{
		float3 pointToLight = AdditionalLights[i].Position.xyz - input.worldPosition;
		if (length(pointToLight) < AdditionalLights[i].Radius)
		{
			float3 lightDirection = normalize(pointToLight);
			intensity += BlinnPhong(normal, input.worldPosition, lightDirection, Diffuse, Specular, Smoothness);
		}
	}

	pixelColor.xyz *= intensity;
	pixelColor += Ambient;

	return pixelColor;
}