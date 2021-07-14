#include "Common.hlsli"

Texture2D AlbedoMap : register(t0);
Texture2D NormalMap : register(t1);
SamplerState Sampler;

cbuffer Properties
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
	float3 mainLight = normalize(float3(0.0f, 1.0f, -1.0f));
	float intensity = BlinnPhong(normal, input.worldPosition, mainLight, Diffuse, Specular, Smoothness);
	pixelColor.xyz *= intensity;
	pixelColor += Ambient;

	return pixelColor;
}