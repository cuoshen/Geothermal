#include "pch.h"
#include "AABB.h"
#include <cmath>

using namespace std;
using namespace DirectX;
using namespace Geothermal;

array<XMFLOAT3, 6> Geothermal::GenerateBoxVertices(AABB box)
{
	return
	{
		XMFLOAT3{box.MinXYZ.x, box.MinXYZ.y, box.MinXYZ.z},
		XMFLOAT3{box.MaxXYZ.x, box.MinXYZ.y, box.MinXYZ.z},
		XMFLOAT3{box.MaxXYZ.x, box.MaxXYZ.y, box.MinXYZ.z},
		XMFLOAT3{box.MaxXYZ.x, box.MaxXYZ.y, box.MaxXYZ.z},
		XMFLOAT3{box.MinXYZ.x, box.MaxXYZ.y, box.MaxXYZ.z},
		XMFLOAT3{box.MinXYZ.x, box.MinXYZ.y, box.MaxXYZ.z}
	};
}

array<XMFLOAT4, 6> Geothermal::GenerateBoxVertices(AABB box, XMMATRIX transform)
{
	array<XMFLOAT3, 6> verticesInModelSpace = GenerateBoxVertices(box);
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

void Geothermal::UpdateBounds(AABB& bounds, XMFLOAT3 point)
{
	XMFLOAT3& newMinXYZ = bounds.MinXYZ;
	newMinXYZ.x = std::min(point.x, newMinXYZ.x);
	newMinXYZ.y = std::min(point.y, newMinXYZ.y);
	newMinXYZ.z = std::min(point.z, newMinXYZ.z);
	XMFLOAT3& newMaxXYZ = bounds.MaxXYZ;
	newMaxXYZ.x = std::max(point.x, newMaxXYZ.x);
	newMaxXYZ.y = std::max(point.y, newMaxXYZ.y);
	newMaxXYZ.z = std::max(point.z, newMaxXYZ.z);
}
