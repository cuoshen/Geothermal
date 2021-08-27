#include "ForwardCommon.hlsli"

struct VertexInput
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texcoord : TEXCOORD;
	float3 tangent : TANGENT;
};

Varyings main(VertexInput input)
{
	Varyings output;
	float4 worldPosition = mul(float4(input.pos, 1.0f), Model2WorldTransform);

	output.clipPosition = mul(worldPosition, World2ClipTransform);
	output.worldPosition = (float3)worldPosition;
	output.lightSpacePosition = mul(worldPosition, World2Light);

	output.normal = normalize
	(
		mul(float4(input.normal, 0.0f), Model2WorldTransform).xyz
	);

	output.tangent = float3(0.0f, 0.0f, 0.0f);
	output.tangent = normalize
	(
		mul(float4(input.tangent, 0.0f), Model2WorldTransform).xyz
	);

	output.texcoord = input.texcoord;

	return output;
}