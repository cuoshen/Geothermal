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

	for (uint i = 0; i < GBufferCount; i++)
	{
		targets[i] = (*(this->sink))[i]->UseAsRenderTarget().get();
	}
}

void DeferredGBufferPass::SetUpPipelineStates()
{
	deviceResources->ResetDefaultPipelineStates();

	// Clear all GBuffers
	for (uint i = 0; i < GBufferCount; i++)
	{
		deviceResources->Context()->ClearRenderTargetView
		(
			(*sink)[i]->UseAsRenderTarget().get(), deviceResources->ClearColor
		);
	}

	deviceResources->Context()->ClearDepthStencilView
	(
		deviceResources->DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0
	);
	deviceResources->Context()->RSSetViewports(1, &(deviceResources->ScreenViewport()));
}

void DeferredGBufferPass::operator()()
{
	SetUpPipelineStates();

	// Bind multiple render targets
	deviceResources->SetTargets(1, targets, deviceResources->DepthStencilView());

	camera->BindCamera2Pipeline();		// Render from the perspective of the main camera

	// First cull objects outside the view frustum, then render pixel attributes onto the GBuffers
	// TODO: sort objects
	for (GameObject*& gameObject : Cull())
	{
		gameObject->Render();
	}
}
