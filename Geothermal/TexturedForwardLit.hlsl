#include "Common.hlsli"

Texture2D AlbedoMap : register(t0);
Texture2D NormalMap : register(t1);
SamplerState Sampler;

cbuffer UnlitProperties
{
	float4 Ambient;
	float4 BaseColor;
	float Diffuse;
	float Specular;
	float Smoothness;
};

float4 main(Varyings input) : SV_TARGET
{
	// Sample textures
	float4 textureColor = AlbedoMap.Sample(Sampler, input.texcoord);
	float4 normalSample = NormalMap.Sample(Sampler, input.texcoord);
	normalSample = (2.0f * normalSample) - 1.0f;

	float3 normal = normalize(input.normal);
	// Apply Gram-Schmidt to make sure tangent is orthogonal to the normal
	float3 tangent = normalize(input.tangent - dot(input.tangent, input.normal) * input.normal);
	float3 biTangent = cross(input.normal, input.tangent);
	float3x3 tangent2World = float3x3(input.tangent, biTangent, input.normal);
	
	normal = normalize(mul(normalSample, tangent2World));

	// Lighting
	float4 pixelColor = BaseColor + textureColor;

	float3 mainLight = normalize(float3(0.0f, 1.0f, -1.0f));
	float intensity = BlinnPhong(normal, input.worldPosition, mainLight, Diffuse, Specular, Smoothness);
	pixelColor.xyz *= intensity;
	pixelColor += Ambient;

	return pixelColor;
}