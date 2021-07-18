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

cbuffer ShadowCasterParameters
{
	matrix World2Light;
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

bool IsInShadow()
{
	return false;
}

float4 main(Varyings input) : SV_TARGET
{
	float4 textureColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 normal = normalize(input.normal);

	// Sample textures if needed
	int useAlbedoMap = TextureFlags & 0x01;
	int useNormalMap = TextureFlags & 0x02;
	int useShadowMap = TextureFlags & 0x03;
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
		if (!IsInShadow())
		{
			intensity = BlinnPhong(normal, input.worldPosition, -MainLight.Direction, Diffuse, Specular, Smoothness);
		}
	}
	else
	{
		intensity = BlinnPhong(normal, input.worldPosition, -MainLight.Direction, Diffuse, Specular, Smoothness);
	}

	pixelColor.xyz *= intensity;
	pixelColor += Ambient;

	return pixelColor;
}