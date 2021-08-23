#include "pch.h"
#include "RenderPass.h"

using namespace std;
using namespace Geothermal;
using namespace Graphics;
using namespace Passes;

RenderPass::RenderPass
(
	shared_ptr<DeviceResources> const& deviceResources,
	std::unique_ptr<std::vector<Texture2D*>> source,
	std::unique_ptr<std::vector<Texture2D*>> sink
) :
	deviceResources(deviceResources), source(move(source)), sink(move(sink))
{
}
