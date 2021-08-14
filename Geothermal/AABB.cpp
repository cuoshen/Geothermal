#include "pch.h"
#include "AABB.h"

using namespace std;
using namespace DirectX;
using namespace Geothermal;

array<XMFLOAT3, 6> Geothermal::GenerateBoxVertices(AABB box)
{
	return array<XMFLOAT3, 6>();
}

array<XMFLOAT3, 6> Geothermal::GenerateBoxVertices(AABB box, XMMATRIX model2world)
{
	array<XMFLOAT3, 6> verticesInModelSpace = GenerateBoxVertices(box);
	array<XMFLOAT3, 6> verticesInWorldSpace;
	for (size_t i = 0; i < 6; i++)
	{
		XMVECTOR modelSpaceCoordinates = XMLoadFloat3(&verticesInModelSpace[i]);
		XMVECTOR worldSpaceCoordinates = XMVector3Transform(modelSpaceCoordinates, model2world);
		XMStoreFloat3(&verticesInWorldSpace[i], worldSpaceCoordinates);
	}

	return verticesInWorldSpace;
}
