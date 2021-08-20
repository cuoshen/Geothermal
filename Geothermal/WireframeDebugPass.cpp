#include "pch.h"
#include "WireframeDebugPass.h"

using namespace Geothermal;
using namespace Graphics;
using namespace Passes;
using namespace std;
using namespace DirectX;

WireframeDebugPass::WireframeDebugPass
(
	shared_ptr<DeviceResources> const& deviceResources, 
	vector<Texture2D*> const& source, 
	vector<Texture2D*> const& sink
) : 
	RenderPass(deviceResources, source, sink),
	camera(nullptr), states(nullptr)
{
	states = make_unique<CommonStates>();
}

void WireframeDebugPass::SetResources(list<GameObject*> renderables, Camera* camera)
{
	this->camera = camera;
	this->renderables = renderables;
}

void WireframeDebugPass::operator()()
{
	SetUpPipelineStates();
	VisualizeBounds();
}

void WireframeDebugPass::SetUpPipelineStates()
{
	deviceResources->Context()->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
	deviceResources->Context()->OMSetDepthStencilState(states->DepthDefault(), 0);

	// Set rasterizer state to wireframe
	deviceResources->Context()->RSSetState(states->Wireframe());

	deviceResources->Context()->ClearRenderTargetView
	(
		sink[0]->UseAsRenderTarget().get(), deviceResources->ClearColor
	);
	deviceResources->Context()->ClearDepthStencilView
	(
		deviceResources->DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0
	);
	deviceResources->Context()->RSSetViewports(1, &(deviceResources->ScreenViewport()));
}

void WireframeDebugPass::VisualizeBounds()
{
}
