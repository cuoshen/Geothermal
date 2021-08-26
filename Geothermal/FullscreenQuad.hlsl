// Self-generated full screen quad vertex shader
#include "DeferredCommon.hlsli"

QuadPixel main(uint vertexID : SV_VertexId)
{
    QuadPixel output;

    float2 texcoord = float2((vertexID << 1) & 2, vertexID & 2);
    output.texcoord = texcoord;
    output.position = float4(texcoord.x * 2 - 1, -texcoord.y * 2 + 1, 0, 1);

    return output;
}
