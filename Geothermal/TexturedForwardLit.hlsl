#include "Common.hlsli"

Texture2D Albedo : register(t0);
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
	float4 textureColor = Albedo.Sample(Sampler, input.texcoord);
	return textureColor;		// DEBUG: visualize texture only
	float4 pixelColor = BaseColor + textureColor;
	float3 normal = normalize(input.normal);

	float3 overheadLight = normalize(float3(0.0f, 1.0f, 1.0f));
	float intensity = BlinnPhong(normal, input.worldPosition, overheadLight, Diffuse, Specular, Smoothness);
	pixelColor.xyz *= intensity;
	pixelColor += Ambient;

	return pixelColor;
}