#include "pch.h"
#include "SceneGeometryPass.h"

using namespace Geothermal;
using namespace Graphics;
using namespace Passes;
using namespace std;

SceneGeometryPass::SceneGeometryPass
(
	shared_ptr<DeviceResources> const& deviceResources, 
	std::unique_ptr<std::vector<Texture2D*>> source,
	std::unique_ptr<std::vector<Texture2D*>> sink
) : 
	RenderPass(deviceResources, move(source), move(sink)), 
	renderables(), camera(nullptr)
{
}

void SceneGeometryPass::SetSceneResources(list<GameObject*> renderables, Camera* camera)
{
	assert(camera != nullptr);
	this->renderables = renderables;
	this->camera = camera;
}

list<GameObject*> SceneGeometryPass::Cull()
{
	list<GameObject*> result;

	bool isInFrustum = false;
	for (GameObject*& renderable : renderables)
	{
		// Get clipping space coordinates
		AABB& boundingBox = renderable->Renderer().Bounds();
		array<XMFLOAT4, BoxVerticesCount> clippingSpaceBoundingBox =
			boundingBox.GenerateBoxVertices(renderable->GetTransform().Object2WorldMatrix() * camera->World2Clip());

		for (XMFLOAT4& clipPosition : clippingSpaceBoundingBox)
		{
			// Do perspective division
			XMFLOAT3 ndcPosition =
			{
				clipPosition.x / clipPosition.w,
				clipPosition.y / clipPosition.w,
				clipPosition.z / clipPosition.w
			};

			// If any of the points of the bound falls into the frustum, the entire shape needs to be rendered
			isInFrustum |= (abs(ndcPosition.x) <= 1) && (abs(ndcPosition.y) <= 1) && (ndcPosition.z >= 0) && (ndcPosition.z <= 1);
		}

		if (isInFrustum)
		{
			result.push_back(renderable);
		}
	}

	return result;
}
