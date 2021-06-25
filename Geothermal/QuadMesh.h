#pragma once
#include "Mesh.h"
#include "DeviceResources.h"

namespace Geothermal::Graphics::Meshes
{
	struct QuadMesh : public Mesh
	{
		QuadMesh(std::shared_ptr<DeviceResources> const& deviceResources);
	};
}
