#include "Common.hlsli"

// Tweak vertex layout by selecting feature sets
// #define PRECOMPUTED_TANGENT

struct VertexInput
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texcoord : TEXCOORD;
#ifdef PRECOMPUTED_TANGENT
	float3 tangent : TANGENT;
#endif
};

Varyings main(VertexInput input)
{
	Varyings output;
	float4 worldPosition = mul(float4(input.pos, 1.0f), Model2WorldTransform);
	output.worldPosition = (float3)worldPosition;
	output.clipPosition = mul(worldPosition, World2ClipTransform);

	output.normal = normalize(
										mul(float4(input.normal, 0.0f), Model2WorldTransform).xyz
									);

	output.tangent = float3(0.0f, 0.0f, 0.0f);
#ifdef PRECOMPUTED_TANGENT
	output.tangent = normalize(
										mul(float4(input.tangent, 0.0f), Model2WorldTransform).xyz
									);
#endif

	output.texcoord = input.texcoord;

	return output;
}