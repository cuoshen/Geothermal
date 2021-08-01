#pragma once
#include "GraphicResources.h"

namespace Geothermal::Graphics::Meshes
{
	struct Mesh
	{
		Mesh() :
			vertices(nullptr)
		{
		}
		std::shared_ptr<Bindables::IndexedVertexBuffer<Structures::VertexPNTT>> vertices;
	};
}
