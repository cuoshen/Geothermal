#include "pch.h"
#include "DeferredLightingPass.h"

using namespace Geothermal;
using namespace Graphics;
using namespace Passes;
using namespace std;
using namespace DirectX;

DeferredLightingPass::DeferredLightingPass
(
	shared_ptr<DeviceResources> const& deviceResources, 
	unique_ptr<vector<Texture2D*>> source,
	unique_ptr<vector<Texture2D*>> sink
) : 
	RenderPass(deviceResources, move(source), move(sink))
{
	states = make_unique<CommonStates>(deviceResources->Device());
}

void DeferredLightingPass::SetDelegates(function<void(void)> uploadShadowResources)
{
	this->uploadShadowResources = uploadShadowResources;
}

void DeferredLightingPass::SetUpPipelineStates()
{
	deviceResources->Context()->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
	deviceResources->Context()->OMSetDepthStencilState(states->DepthNone(), 0);
	deviceResources->Context()->RSSetState(states->CullNone());
	deviceResources->Context()->RSSetViewports(1, &(deviceResources->ScreenViewport()));
}

void DeferredLightingPass::operator()()
{
	SetUpPipelineStates();

	// Execute full screen lighting pass

}
