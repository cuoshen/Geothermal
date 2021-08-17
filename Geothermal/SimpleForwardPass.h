#pragma once
#include <functional>
#include "GraphicResources.h"
#include "RenderPass.h"
#include "GameObject.h"
#include "Camera.h"

namespace Geothermal::Graphics::Passes
{
	class SimpleForwardPass : public RenderPass
	{
	public:
		/// <summary>
		/// Draw all renderables in a simple forward pass.
		/// </summary>
		/// <param name="sink">[0]: HDR scene buffer that receives the rendered scene image</param>
		SimpleForwardPass
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			std::vector<Texture2D*> const& source,
			std::vector<Texture2D*> const& sink
		);

		void SetResources
		(
			std::list<GameObject*> renderables,
			Camera* camera,
			std::function<void(void)> uploadShadowResources,
			std::function<void(void)> uploadLightingResources
		);

		void operator()() override;

	protected:
		/// <summary>
		/// Apply a frustum culling to renderables
		/// </summary>
		/// <returns>A list of game objects visible in the frustum</returns>
		std::list<GameObject*> Cull();

		/// <summary>
		/// Sort opaque game objects from near to far
		/// </summary>
		/// <returns>List of sorted opaque objects</returns>
		std::list<GameObject*> SortOpaque();

		/// <summary>
		/// Visualize renderable bounding boxes in wireframe
		/// </summary>
		void VisualizeBounds();

		std::list<GameObject*> renderables;
		Camera* camera;
		std::function<void(void)> uploadShadowResources;
		std::function<void(void)> uploadLightingResources;
	};
}
