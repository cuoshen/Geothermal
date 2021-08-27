#include "Lighting.hlsli"
#include "Shadow.hlsli"
#include "DeferredCommon.hlsli"

Texture2D GBuffer0 : register(GBUFFER_0_SLOT);
Texture2D GBuffer1 : register(GBUFFER_1_SLOT);
Texture2D GBufferDepth : register(GBUFFER_DEPTH_SLOT);
SamplerState Sampler;

float4 main(QuadPixel input) : SV_TARGET
{
	float4 g0 = GBuffer0.Sample(Sampler, input.texcoord);
	float4 g1 = GBuffer1.Sample(Sampler, input.texcoord);
	float depth = GBufferDepth.Sample(Sampler, input.texcoord);

	float3 albedo = g0.xyz;
	float3 worldSpaceNormal = g1.xyz;
	float smoothness = g1.w;
	// Reconstruct world position
	float3 worldPosition = ReconstructWorldPosition(input.texcoord, depth);
	float4 lightSpacePosition = mul(float4(worldPosition, 1.0f), World2Light);
	
	float intensity = 0.0f;
	// Shade directional light
	if (!IsInShadow(lightSpacePosition, Sampler))
	{
		intensity +=
				BlinnPhong
				(
					worldSpaceNormal, worldPosition, -MainLight.Direction,
					0.5f, 0.5f, smoothness, CameraWorldPosition
				);
	}
	
	float3 color = albedo * intensity + Ambience;

	return float4(worldSpaceNormal, 1.0f);
}
