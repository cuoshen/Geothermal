#include "pch.h"
#include "AABB.h"
#include <cmath>

using namespace std;
using namespace DirectX;
using namespace Geothermal;

array<XMFLOAT3, 6> Geothermal::AABB::GenerateBoxVertices()
{
	return
	{
		XMFLOAT3{MinXYZ.x, MinXYZ.y, MinXYZ.z},
		XMFLOAT3{MaxXYZ.x, MinXYZ.y, MinXYZ.z},
		XMFLOAT3{MaxXYZ.x, MaxXYZ.y, MinXYZ.z},
		XMFLOAT3{MaxXYZ.x, MaxXYZ.y, MaxXYZ.z},
		XMFLOAT3{MinXYZ.x, MaxXYZ.y, MaxXYZ.z},
		XMFLOAT3{MinXYZ.x, MinXYZ.y, MaxXYZ.z}
	};
}

array<XMFLOAT4, 6> Geothermal::AABB::GenerateBoxVertices(XMMATRIX transform)
{
	array<XMFLOAT3, 6> verticesInModelSpace = GenerateBoxVertices();
	array<XMFLOAT4, 6> verticesTransformed;
	for (size_t i = 0; i < 6; i++)
	{
		XMFLOAT4 modelSpacePosition =
			XMFLOAT4{ verticesInModelSpace[i].x, verticesInModelSpace[i].y, verticesInModelSpace[i].z, 1.0f };

		XMVECTOR modelSpaceCoordinates = XMLoadFloat4(&modelSpacePosition);
		XMVECTOR transformed = XMVector4Transform(modelSpaceCoordinates, transform);
		XMStoreFloat4(&verticesTransformed[i], transformed);
	}

	return verticesTransformed;
}

void Geothermal::AABB::UpdateBounds(XMFLOAT3 point)
{
	MinXYZ.x = std::min(point.x, MinXYZ.x);
	MinXYZ.y = std::min(point.y, MinXYZ.y);
	MinXYZ.z = std::min(point.z, MinXYZ.z);
	MaxXYZ.x = std::max(point.x, MaxXYZ.x);
	MaxXYZ.y = std::max(point.y, MaxXYZ.y);
	MaxXYZ.z = std::max(point.z, MaxXYZ.z);
}
