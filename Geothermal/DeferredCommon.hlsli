#ifndef GEOTHERMAL_DEFERRED_COMMON
#define GEOTHERMAL_DEFERRED_COMMON

#include "SlotConfig.hlsli"

struct QuadPixel
{
	float4 position : SV_Position;
	float2 texcoord : TEXCOORD;
};

cbuffer DeferredViewParameters : register(VIEW_PARAMETERS_SLOT)
{
	matrix Clip2WorldTransform;
	float3 CameraWorldPosition;
};

cbuffer DeferredParameters : register(DEFERRED_PARAMETERS_SLOT)
{
	float3 Ambience;
};

float3 ReconstructWorldPosition(float2 screenSpacePosition, float depth)
{
	return float3(0.0f, 0.0f, 0.0f);
}

#endif