#include "Common.hlsli"

Texture2D AlbedoMap : register(ALBEDO_MAP_SLOT);
Texture2D NormalMap : register(NORMAL_MAP_SLOT);
SamplerState Sampler;

cbuffer Properties : register(PROPERTIES_SLOT)
{
	MaterialProperty Property;
};

struct GBufferOutput
{
	float4 g0 : SV_Target0;
	float4 g1 : SV_Target1;
};

GBufferOutput main(Varyings input)
{
	GBufferOutput output;

	float4 textureColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 normal;

	// Parse texture flags
	int useAlbedoMap = Property.textureFlags & 0x01;
	int useNormalMap = Property.textureFlags & 0x02;
	int useShadowMap = Property.textureFlags & 0x04;

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
	else
	{
		normal = normalize(input.normal);
	}

	float4 albedo = Property.baseColor + textureColor + Property.ambient;
	output.g0.xyz = albedo.xyz;
	output.g1.xyz = normal.xyz;
	output.g1.w = Property.smoothness;

	return output;
}
