#ifndef GEOTHERMAL_DEFERRED_COMMON
#define GEOTHERMAL_DEFERRED_COMMON

struct QuadPixel
{
	float4 position : SV_Position;
	float2 texcoord : TEXCOORD;
};

#endif