#include "Common.hlsli"

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
	float4 pixelColor = BaseColor;
	float3 normal = normalize(input.normal);

	float3 overheadLight = normalize(float3(0.0f, 1.0f, 1.0f));
	float intensity = BlinnPhong(normal, input.worldPosition, overheadLight, Diffuse, Specular, Smoothness);
	pixelColor.xyz *= intensity;
	pixelColor += Ambient;

	return pixelColor;
}