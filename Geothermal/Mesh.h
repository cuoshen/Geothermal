#pragma once
#include "GraphicResources.h"

namespace Geothermal::Graphics::Meshes
{
	struct Mesh
	{
		std::shared_ptr<Bindables::IndexedVertexBuffer<Structures::VertexPNTT>> vertices;
		std::shared_ptr<AABB> bounds;
	};
}
