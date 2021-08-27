#ifndef GEOTHERMAL_DEFERRED_COMMON
#define GEOTHERMAL_DEFERRED_COMMON

#include "SlotConfig.hlsli"

struct QuadPixel
{
	float4 position : SV_Position;
	float2 texcoord : TEXCOORD;
};

float3 ReconstructWorldPosition(float2 screenSpacePosition, float depth)
{
	return float3(0.0f, 0.0f, 0.0f);
}

#endif