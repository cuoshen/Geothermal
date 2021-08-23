#include "Common.hlsli"

Texture2D AlbedoMap : register(ALBEDO_MAP_SLOT);
Texture2D NormalMap : register(NORMAL_MAP_SLOT);
SamplerState Sampler;

cbuffer Properties : register(PROPERTIES_SLOT)
{
	MaterialProperty property;
};

struct GBufferOutput
{
	float4 g0 : SV_Target0;
	float4 g1 : SV_Target1;
};

GBufferOutput main(Varyings input)
{
	GBufferOutput output;

	return output;
}
