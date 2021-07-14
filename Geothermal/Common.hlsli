#ifndef GEOTHERMAL_SHADING_COMMON
#define GEOTHERMAL_SHADING_COMMON

// Each model is responsible for supplying its own transform
cbuffer TransformBuffer : register(b0)
{
	matrix Model2WorldTransform;
};

// Camera supplies the world 2 clip transform as well as its position
cbuffer CameraParameters : register(b1)
{
	matrix World2ClipTransform;
	float3 CameraWorldPosition;
};

struct Varyings
{
	float4 clipPosition					:	SV_POSITION;
	float3 worldPosition				:	WORLD_POSITION;
	float2 texcoord							:	TEXCOORD;
	float3 normal							:	NORMAL;
	float3 tangent							:	TANGENT;
};

struct VertexColoredVaryings
{
	float4 clipPosition					:	SV_POSITION;
	float3 worldPosition				:	WORLD_POSITION;
	float4 vertexColor					:	VERTEX_COLOR;
	float3 normal							:	NORMAL;
};

float Lambert(float3 normal, float3 lightDirection, float diffuseStrength)
{
	float diffuse = max(
		0.0f,
		dot(normal, lightDirection)
	) * diffuseStrength;
	return diffuse;
}

float BlinnPhong(float3 normal, float3 worldPosition, float3 lightDirection, float diffuseStrength, float specularStrength, float smoothness)
{
	float3 viewDirection = normalize(CameraWorldPosition - worldPosition);
	float3 halfway = normalize(lightDirection + viewDirection);

	float diffuse = Lambert(normal, lightDirection, diffuseStrength);

	float specular = pow(
			max(0.0f, dot(normal, halfway)), smoothness
	) * specularStrength;

	return diffuse + specular;
}

#endif