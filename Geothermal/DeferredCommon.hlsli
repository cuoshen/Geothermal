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
	float x = screenSpacePosition.x * 2 - 1;
	float y = (1 - screenSpacePosition.y) * 2 - 1;
	float4 clipPosition = float4(x, y, depth, 1.0f);
	float4 worldPosition = mul(clipPosition, ViewParameters.clip2WorldTransform);

	return (worldPosition.xyz / worldPosition.w);
}

#endif