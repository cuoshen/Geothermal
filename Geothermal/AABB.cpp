#include "pch.h"
#include "AABB.h"

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

array<XMFLOAT3, 6> Geothermal::GenerateBoxVertices(AABB box, XMMATRIX transform)
{
	array<XMFLOAT3, 6> verticesInModelSpace = GenerateBoxVertices(box);
	array<XMFLOAT3, 6> verticesInWorldSpace;
	for (size_t i = 0; i < 6; i++)
	{
		XMVECTOR modelSpaceCoordinates = XMLoadFloat3(&verticesInModelSpace[i]);
		XMVECTOR worldSpaceCoordinates = XMVector3Transform(modelSpaceCoordinates, transform);
		XMStoreFloat3(&verticesInWorldSpace[i], worldSpaceCoordinates);
	}

	return verticesInWorldSpace;
}
