#include "Common.hlsli"

struct VertexInput
{
	float3 pos : POSITION;
};

float4 main(VertexInput input) : SV_POSITION
{
	float4 worldPosition = mul(float4(input.pos, 1.0f), Model2WorldTransform);
	float4 clipPosition = mul(worldPosition, World2ClipTransform);

	return clipPosition;
}