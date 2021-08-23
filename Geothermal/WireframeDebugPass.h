#pragma once
#include <functional>
#include "GraphicResources.h"
#include "RenderPass.h"
#include "SceneGeometryPass.h"

namespace Geothermal::Graphics::Passes
{
	/// <summary>
	/// Visualize all scene object bounds with wireframe.
	/// Does not use a source or sink as it writes directly to the back buffer.
	/// </summary>
	class WireframeDebugPass : public SceneGeometryPass
	{
	public:
		WireframeDebugPass
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			std::unique_ptr<std::vector<Texture2D*>> source,
			std::unique_ptr<std::vector<Texture2D*>> sink
		);

		void operator()() override;

	protected:
		std::unique_ptr<DirectX::CommonStates> states;
		std::unique_ptr<Materials::Material> wireframeMaterial;

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
