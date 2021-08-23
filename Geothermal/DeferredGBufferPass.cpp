#include "pch.h"
#include "DeferredGBufferPass.h"

using namespace Geothermal;
using namespace Graphics;
using namespace Passes;
using namespace std;

DeferredGBufferPass::DeferredGBufferPass
(
	shared_ptr<DeviceResources> const& deviceResources, 
	unique_ptr<vector<Texture2D*>> sink
) : 
	SceneGeometryPass(deviceResources, nullptr, move(sink))
{
}

void DeferredGBufferPass::operator()()
{
}
