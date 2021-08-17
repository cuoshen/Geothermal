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
	vector<Texture2D*> const& source,
	vector<Texture2D*> const& sink
) :
	RenderPass(deviceResources, source, sink), renderables(),
	camera(nullptr), uploadLightingResources(nullptr), uploadShadowResources(nullptr)
{
}

void SimpleForwardPass::SetResources
(
	list<GameObject*> renderables, Camera* camera,
	function<void(void)> uploadShadowResources,
	function<void(void)> uploadLightingResources
)
{
	assert(camera != nullptr);
	this->renderables = renderables;
	this->camera = camera;
	this->uploadShadowResources = uploadShadowResources;
	this->uploadLightingResources = uploadLightingResources;
}

list<GameObject*> SimpleForwardPass::Cull()
{
	list<GameObject*> result;

	bool isInFrustum = false;
	for (GameObject*& renderable : renderables)
	{
		// Get clipping space coordinates
		AABB& boundingBox = renderable->Renderer().Bounds();
		array<XMFLOAT4, 6> clippingSpaceBoundingBox = 
			GenerateBoxVertices
			(
				boundingBox,
				renderable->GetTransform().Object2WorldMatrix() * camera->World2Clip()
			);

		for (XMFLOAT4& clipPosition : clippingSpaceBoundingBox)
		{
			XMFLOAT3 ndcPosition = { clipPosition.x / clipPosition.w, clipPosition.y / clipPosition.w, clipPosition.z / clipPosition.w };
			isInFrustum |= (abs(ndcPosition.x) <= 1) && (abs(ndcPosition.y) <= 1) && (ndcPosition.z >= 0) && (ndcPosition.z <= 1);
		}

		if (isInFrustum)
		{
			result.push_back(renderable);
		}
	}

	return result;
}

void SimpleForwardPass::operator()()
{
	deviceResources->ResetDefaultPipelineStates();
	deviceResources->Context()->ClearRenderTargetView
	(
		sink[0]->UseAsRenderTarget().get(), deviceResources->ClearColor
	);
	deviceResources->Context()->ClearDepthStencilView
	(
		deviceResources->DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0
	);
	deviceResources->Context()->RSSetViewports(1, &(deviceResources->ScreenViewport()));
	ID3D11RenderTargetView* target = sink[0]->UseAsRenderTarget().get();
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
