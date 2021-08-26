#include "pch.h"
#include "DeferredLightingPass.h"

using namespace Geothermal;
using namespace Graphics;
using namespace Passes;
using namespace std;

DeferredLightingPass::DeferredLightingPass
(
	shared_ptr<DeviceResources> const& deviceResources, 
	unique_ptr<vector<Texture2D*>> source,
	unique_ptr<vector<Texture2D*>> sink
) : 
	RenderPass(deviceResources, move(source), move(sink))
{
}

void DeferredLightingPass::SetDelegates(function<void(void)> uploadShadowResources)
{
	this->uploadShadowResources = uploadShadowResources;
}

void DeferredLightingPass::SetUpPipelineStates()
{

}

void DeferredLightingPass::operator()()
{
}
