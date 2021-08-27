#pragma once
#include "GraphicResources.h"

namespace Geothermal::Graphics::Passes
{
	/// <summary>
	/// Abstract class for a render pass which is a functional object 
	/// which can be executed in the pipeline
	/// </summary>
	class RenderPass
	{
	public:

		RenderPass
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			std::unique_ptr<std::vector<Texture2D*>> source,
			std::unique_ptr<std::vector<Texture2D*>> sink
		);

		/// <summary>
		/// Execute the pass
		/// </summary>
		virtual void operator()() = 0;

	protected:
		std::shared_ptr<DeviceResources> deviceResources;

		virtual void SetUpPipelineStates() = 0;

		// Render pass really does not OWN the textures involved as source or sink
		// it just need to keep some reference to them in the form of raw pointers
		std::unique_ptr<std::vector<Texture2D*>> source;
		std::unique_ptr<std::vector<Texture2D*>> sink;
	};
}
