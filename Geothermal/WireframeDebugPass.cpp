#include "pch.h"
#include "WireframeDebugPass.h"

using namespace Geothermal;
using namespace Graphics;
using namespace Passes;
using namespace std;


WireframeDebugPass::WireframeDebugPass
(
	std::shared_ptr<DeviceResources> const& deviceResources, 
	std::vector<Texture2D*> const& source, 
	std::vector<Texture2D*> const& sink
) : 
	RenderPass(deviceResources, source, sink),
	camera(nullptr)
{
}

void WireframeDebugPass::operator()()
{
	VisualizeBounds();
}

void Geothermal::Graphics::Passes::WireframeDebugPass::SetUpPipelineStates()
{
}

void WireframeDebugPass::VisualizeBounds()
{
}
