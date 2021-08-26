#include "Common.hlsli"
#include "DeferredCommon.hlsli"

Texture2D GBuffer0 : register(GBUFFER_0_SLOT);
Texture2D GBuffer1 : register(GBUFFER_1_SLOT);
Texture2D GBufferDepth : register(GBUFFER_DEPTH_SLOT);
SamplerState Sampler;

float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
