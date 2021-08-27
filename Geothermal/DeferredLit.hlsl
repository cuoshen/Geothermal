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
	// Reconstruct world position
	float3 worldPosition = ReconstructWorldPosition(input.texcoord, depth);

	return float4(worldSpaceNormal, 1.0f);
}
