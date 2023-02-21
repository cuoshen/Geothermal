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
	assert(this->sink != nullptr);
	assert(this->sink->size() == GBufferCount);

	for (u32 i = 0; i < GBufferRenderTargetCount; i++)
	{
		renderTargets[i] = (*(this->sink))[i]->UseAsRenderTarget().get();
	}
	// The last one in the GBuffer is always the depth stencil view
	depthStencil = (*(this->sink))[GBufferCount - 1]->UseAsDepthStencil().get();
}

void DeferredGBufferPass::SetUpPipelineStates()
{
	deviceResources->ResetDefaultPipelineStates();

	// Clear all GBuffer RTVs
	for (u32 i = 0; i < GBufferRenderTargetCount; i++)
	{
		deviceResources->Context()->ClearRenderTargetView
		(
			(*sink)[i]->UseAsRenderTarget().get(), deviceResources->ClearColor
		);
	}

	// Clear GBuffer depth map
	deviceResources->Context()->ClearDepthStencilView
	(
		depthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0
	);

	deviceResources->Context()->RSSetViewports(1, &(deviceResources->ScreenViewport()));
}

void DeferredGBufferPass::operator()()
{
	SetUpPipelineStates();

	// Bind GBuffers
	deviceResources->SetTargets(GBufferRenderTargetCount, renderTargets, depthStencil);

	camera->BindCamera2Pipeline();		// Render from the perspective of the main camera

	// First cull objects outside the view frustum, then render pixel attributes onto the GBuffers
	// TODO: sort objects
	for (GameObject*& gameObject : Cull())
	{
		gameObject->Render();
	}
}
