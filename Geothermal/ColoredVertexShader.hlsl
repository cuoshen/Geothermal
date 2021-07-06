#include "Common.hlsli"

struct VertexInput
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float4 vertexColor : VERTEX_COLOR;
};

VertexColoredVaryings main(VertexInput input)
{
	VertexColoredVaryings output;
	float4 worldPosition = mul(float4(input.pos, 1.0f), Model2WorldTransform);
	output.worldPosition = (float3)worldPosition;
	output.clipPosition = mul(worldPosition, World2ClipTransform);

	output.normal = normalize(
		mul(float4(input.normal, 0.0f), Model2WorldTransform)
	);

	output.vertexColor = input.vertexColor;

	return output;
}