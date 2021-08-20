#pragma once
#include <functional>
#include "GraphicResources.h"
#include "RenderPass.h"
#include "GameObject.h"
#include "Camera.h"

namespace Geothermal::Graphics::Passes
{
	class WireframeDebugPass : public RenderPass
	{
	public:
		WireframeDebugPass
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			std::vector<Texture2D*> const& source,
			std::vector<Texture2D*> const& sink
		);

		void SetResources
		(
			std::list<GameObject*> renderables,
			Camera* camera
		);

		void operator()() override;

	protected:
		std::list<GameObject*> renderables;
		Camera* camera;
		std::unique_ptr<DirectX::CommonStates> states;

		/// <summary>
		/// Have the rasterizer draw wireframes
		/// </summary>
		void SetUpPipelineStates() override;
		/// <summary>
		/// Visualize renderable bounding boxes in wireframe
		/// </summary>
		void VisualizeBounds();
	};
}
