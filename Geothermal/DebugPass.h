#pragma once
#include "GraphicResources.h"
#include "RenderPass.h"

namespace Geothermal::Graphics::Passes
{
	class DebugPass : public RenderPass
	{
	public:
		DebugPass
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			std::vector<Texture2D*> const& source,
			std::vector<Texture2D*> const& sink
		);

		void operator()() override;

	protected:
		/// <summary>
		/// Visualize renderable bounding boxes in wireframe
		/// </summary>
		void VisualizeBounds();
	};
}