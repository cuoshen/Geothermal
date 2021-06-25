#include "Common.hlsli"

struct VertexInput
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texcoord : TEXCOORD;
};

Varyings main(VertexInput input)
{
	Varyings output;
	float4 worldPosition = mul(float4(input.pos, 1.0f), Model2WorldTransform);
	output.worldPosition = (float3)worldPosition;
	output.clipPosition = mul(worldPosition, World2ClipTransform);

	output.normal = normalize(
										mul(float4(input.normal, 0.0f), Model2WorldTransform)
									);
	// TODO: transfer uv
	output.texcoord = input.texcoord;

	return output;
}