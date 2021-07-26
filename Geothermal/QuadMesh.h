#pragma once
#include "GraphicResources.h"
#include "Mesh.h"

namespace Geothermal::Graphics::Meshes
{
	struct QuadMesh : public Mesh
	{
		QuadMesh(std::shared_ptr<DeviceResources> const& deviceResources);
	};
}
