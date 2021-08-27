#ifndef GEOTHERMAL_DEFERRED_COMMON
#define GEOTHERMAL_DEFERRED_COMMON

#include "SlotConfig.hlsli"
#include "Lighting.hlsli"
#include "Shadow.hlsli"

struct QuadPixel
{
	float4 position : SV_Position;
	float2 texcoord : TEXCOORD;
};

struct DeferredViewParameters
{
	matrix clip2WorldTransform;
	float3 cameraWorldPosition;
	float padding;
};

cbuffer DeferredParameters : register(DEFERRED_PARAMETERS_SLOT)
{
	float3 Ambience;
	float Padding0;
	DirectionalLight MainLight;
	DeferredViewParameters ViewParameters;
};

float3 ReconstructWorldPosition(float2 screenSpacePosition, float depth)
{
	return float3(0.0f, 0.0f, 0.0f);
}

#endif