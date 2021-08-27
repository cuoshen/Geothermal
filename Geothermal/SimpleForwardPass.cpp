#include "pch.h"
#include "SimpleForwardPass.h"
#include <math.h>

using namespace std;
using namespace Geothermal;
using namespace Graphics;
using namespace Passes;

SimpleForwardPass::SimpleForwardPass
(
	shared_ptr<DeviceResources> const& deviceResources,
	unique_ptr<vector<Texture2D*>> source,
	unique_ptr<vector<Texture2D*>> sink
) :
	SceneGeometryPass(deviceResources, move(source), move(sink)),
	uploadLightingResources(nullptr), uploadShadowResources(nullptr)
{
}

void SimpleForwardPass::SetDelegates
(
	function<void(void)> uploadShadowResources,
	function<void(void)> uploadLightingResources
)
{
	this->uploadShadowResources = uploadShadowResources;
	this->uploadLightingResources = uploadLightingResources;
}

void SimpleForwardPass::SetUpPipelineStates()
{
	deviceResources->ResetDefaultPipelineStates();
	deviceResources->Context()->ClearRenderTargetView
	(
		(*sink)[0]->UseAsRenderTarget().get(), deviceResources->ClearColor
	);
	deviceResources->Context()->ClearDepthStencilView
	(
		deviceResources->DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0
	);
	deviceResources->Context()->RSSetViewports(1, &(deviceResources->ScreenViewport()));
}

void SimpleForwardPass::operator()()
{
	SetUpPipelineStates();

	ID3D11RenderTargetView* target = (*sink)[0]->UseAsRenderTarget().get();
	deviceResources->SetTargets(1, &target, deviceResources->DepthStencilView());

	camera->BindCamera2Pipeline();		// Render from the perspective of the main camera

	// have core render pipeline upload the shadow and lights to GPU
	uploadShadowResources();
	uploadLightingResources();

	// First cull objects outside the view frustum, then render them in a single pass
	// TODO: sort objects
	for (GameObject*& gameObject : Cull())
	{
		gameObject->Render();
	}
}
