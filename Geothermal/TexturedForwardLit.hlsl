#include "Common.hlsli"

Texture2D AlbedoMap : register(t0);
Texture2D NormalMap : register(t1);
SamplerState Sampler;

cbuffer UnlitProperties
{
	float4		Ambient;
	float4		BaseColor;
	float		Diffuse;
	float		Specular;
	float		Smoothness;
};

float3 ComputeWorldSpaceNormal(float3 pixelNormal, float3 pixelTangent, float4 normalSample) 
{
	normalSample = (2.0f * normalSample) - 1.0f;		// Bring sample range from [0,1] to [-1,1]

	pixelNormal = normalize(pixelNormal);
	// Apply Gram-Schmidt to make sure tangent is orthogonal to the normal
	pixelTangent = normalize(pixelTangent - dot(pixelTangent, pixelNormal) * pixelNormal);
	float3 biTangent = cross(pixelNormal, pixelTangent);
	float3x3 tangent2World = float3x3(pixelTangent, biTangent, pixelNormal);

	return normalize(mul(normalSample.xyz , tangent2World));
}

float4 main(Varyings input) : SV_TARGET
{
	// Sample textures
	float4 textureColor = AlbedoMap.Sample(Sampler, input.texcoord);
	float4 normalSample = NormalMap.Sample(Sampler, input.texcoord);

	// Compute normal
	float3 normal = ComputeWorldSpaceNormal(input.normal, input.tangent, normalSample);

	// Lighting
	float4 pixelColor = BaseColor + textureColor;
	float3 mainLight = normalize(float3(0.0f, 1.0f, -1.0f));
	float intensity = BlinnPhong(normal, input.worldPosition, mainLight, Diffuse, Specular, Smoothness);
	pixelColor.xyz *= intensity;
	pixelColor += Ambient;

	return pixelColor;
}