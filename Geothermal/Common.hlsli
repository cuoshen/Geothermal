#ifndef GEOTHERMAL_SHADING_COMMON
#define GEOTHERMAL_SHADING_COMMON

#include "SlotConfig.hlsli"

// Each model is responsible for supplying its own transform
cbuffer TransformBuffer : register(TRANSFORM_BUFFER_SLOT)
{
	matrix Model2WorldTransform;
};

// Camera supplies the world 2 clip transform as well as its position
cbuffer ViewParameters : register(VIEW_PARAMETERS_SLOT)
{
	matrix World2ClipTransform;
	float3 CameraWorldPosition;
};

cbuffer ShadowCasterParameters : register(SHADOW_TRANSFORM_SLOT)
{
	matrix World2Light;
};

struct Varyings
{
	float4 clipPosition					:	SV_POSITION;
	float3 worldPosition				:	WORLD_POSITION;
	float2 texcoord						:	TEXCOORD;
	float3 normal							:	NORMAL;
	float3 tangent							:	TANGENT;
	float4 lightSpacePosition		:	LIGHT_SPACE_POSITION;
};

struct VertexColoredVaryings
{
	float4 clipPosition					:	SV_POSITION;
	float3 worldPosition				:	WORLD_POSITION;
	float4 vertexColor					:	VERTEX_COLOR;
	float3 normal							:	NORMAL;
};

#include "Lighting.hlsli"
#include "Shadow.hlsli"

float3 ComputeWorldSpaceNormal(float3 pixelNormal, float3 pixelTangent, float3 normalSample)
{
	normalSample = (2.0f * normalSample) - 1.0f;		// Bring sample range from [0,1] to [-1,1]

	pixelNormal = normalize(pixelNormal);
	// Apply Gram-Schmidt to make sure tangent is orthogonal to the normal
	pixelTangent = normalize(pixelTangent - dot(pixelTangent, pixelNormal) * pixelNormal);
	float3 biTangent = cross(pixelNormal, pixelTangent);
	float3x3 tangent2World = float3x3(pixelTangent, biTangent, pixelNormal);

	return normalize(mul(normalSample, tangent2World));
}

#endif