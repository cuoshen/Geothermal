#pragma once
#include "VertexBuffer.h"
#include "Vertices.h"

namespace Geothermal::Graphics::Meshes
{
	struct Mesh
	{
		Mesh():
			vertices(nullptr)
		{
		}
		std::shared_ptr<Bindables::IndexedVertexBuffer<Structures::VertexPNT>> vertices;
	};
}
