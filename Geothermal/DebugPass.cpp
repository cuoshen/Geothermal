#include "pch.h"
#include "DebugPass.h"

using namespace Geothermal;
using namespace Graphics;
using namespace Passes;
using namespace std;


DebugPass::DebugPass
(
	std::shared_ptr<DeviceResources> const& deviceResources, 
	std::vector<Texture2D*> const& source, 
	std::vector<Texture2D*> const& sink
) : 
	RenderPass(deviceResources, source, sink)
{
}

void Geothermal::Graphics::Passes::DebugPass::operator()()
{
}

void Geothermal::Graphics::Passes::DebugPass::VisualizeBounds()
{
}
