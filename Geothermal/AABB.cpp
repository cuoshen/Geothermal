#include "pch.h"
#include "AABB.h"
#include <cmath>
#include "VertexBuffer.h"
#include "Vertices.h"

using namespace std;
using namespace DirectX;
using namespace Geothermal;

array<XMFLOAT3, 6> AABB::GenerateBoxVertices()
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

array<XMFLOAT4, 6> AABB::GenerateBoxVertices(XMMATRIX transform)
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

void AABB::UpdateBounds(XMFLOAT3 point)
{
	MinXYZ.x = std::min(point.x, MinXYZ.x);
	MinXYZ.y = std::min(point.y, MinXYZ.y);
	MinXYZ.z = std::min(point.z, MinXYZ.z);
	MaxXYZ.x = std::max(point.x, MaxXYZ.x);
	MaxXYZ.y = std::max(point.y, MaxXYZ.y);
	MaxXYZ.z = std::max(point.z, MaxXYZ.z);
}

void AABB::DrawWireFrame(shared_ptr<Graphics::DeviceResources> deviceResources, XMMATRIX transform)
{
	// Build wireframe vertex buffer
	array<XMFLOAT4, 6> boxVertices = this->GenerateBoxVertices(transform);
	vector<Graphics::Structures::VertexP> wireframeVertices;

	for (uint i = 0; i < BoxVerticesCount; i++)
	{
		for (uint j = i; j < BoxVerticesCount; j++)
		{
			// Add a line between v[i] and v[j]
			wireframeVertices.push_back(Graphics::Structures::VertexP{ {boxVertices[i].x, boxVertices[i].y, boxVertices[i].z} });
			wireframeVertices.push_back(Graphics::Structures::VertexP{ {boxVertices[j].x, boxVertices[j].y, boxVertices[j].z} });
		}
	}
}
