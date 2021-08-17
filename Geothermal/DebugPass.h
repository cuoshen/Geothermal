#pragma once
#include <functional>
#include "GraphicResources.h"
#include "RenderPass.h"
#include "SimpleForwardPass.h"
#include "GameObject.h"
#include "Camera.h"

namespace Geothermal::Graphics::Passes
{
	class DebugPass : public SimpleForwardPass
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
