#include "pch.h"
#include "AABB.h"
#include <cmath>
#include "VertexBuffer.h"
#include "Vertices.h"
#include "ConstantBuffer.h"

using namespace std;
using namespace DirectX;
using namespace Geothermal;

array<XMFLOAT3, BoxVerticesCount> AABB::GenerateBoxVertices()
{
	array<XMFLOAT3, BoxVerticesCount> corners;
	for (u32 i = 0; i < BoxVerticesCount; i++)
	{
		XMFLOAT3 point;
		point.x = (i & 1) ? MinXYZ.x : MaxXYZ.x;
		point.y = (i & 2) ? MinXYZ.y : MaxXYZ.y;
		point.z = (i & 4) ? MinXYZ.z : MaxXYZ.z;
		corners[i] = point;
	}
	return corners;
}

array<XMFLOAT4, BoxVerticesCount> AABB::GenerateBoxVertices(XMMATRIX transform)
{
	array<XMFLOAT3, BoxVerticesCount> verticesInModelSpace = GenerateBoxVertices();
	array<XMFLOAT4, BoxVerticesCount> verticesTransformed;
	for (size_t i = 0; i < BoxVerticesCount; i++)
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
	vector<Graphics::Structures::VertexP> wireframeVertices;

	array<XMFLOAT3, BoxVerticesCount> boxVertices = this->GenerateBoxVertices();
	for (auto& position : boxVertices)
	{
		wireframeVertices.push_back({ position });
	}
	vector<UINT> indices;

	for (u32 i = 0; i < BoxVerticesCount; i++)
	{
		for (u32 j = 0; j < BoxVerticesCount; j++)
		{
			if 
			(
				((i ^ j) == 1) || ((i ^ j) == 2) || ((i ^ j) == 4)
			)
			{
				// Only add a line between neighbouring corners
				indices.push_back(i);
				indices.push_back(j);
			}
		}
	}

	Graphics::Bindables::IndexedVertexBuffer<Graphics::Structures::VertexP> wireframe(deviceResources, wireframeVertices, indices);
	wireframe.Bind();
	deviceResources->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	Graphics::Bindables::VertexConstantBuffer<XMMATRIX> object2WorldTransformBuffer(deviceResources, XMMatrixTranspose(transform),0u);
	object2WorldTransformBuffer.Bind();

	deviceResources->Context()->DrawIndexed(wireframe.GetIndexCount(), 0u, 0u);
}
