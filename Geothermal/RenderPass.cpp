#include "pch.h"
#include "RenderPass.h"

using namespace std;
using namespace Geothermal;
using namespace Graphics;
using namespace Passes;

RenderPass::RenderPass
(
	shared_ptr<DeviceResources> const& deviceResources, 
	vector<Texture2D*> const& source,
	vector<Texture2D*> const& sink
):
	deviceResources(deviceResources), source(source), sink(sink)
{
}
