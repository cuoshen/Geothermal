#pragma once
#include <functional>
#include "GraphicResources.h"
#include "RenderPass.h"
#include "SceneGeometryPass.h"
#include "GameObject.h"

namespace Geothermal::Graphics::Passes
{
	/// <summary>
	/// Draw all renderables in a simple forward pass.
	/// </summary>
	class SimpleForwardPass : public SceneGeometryPass
	{
	public:
		/// <param name="sink">[0]: HDR scene buffer that receives the rendered scene image</param>
		SimpleForwardPass
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			std::vector<Texture2D*> const& source,
			std::vector<Texture2D*> const& sink
		);

		void SetDelegates
		(
			std::function<void(void)> uploadShadowResources,
			std::function<void(void)> uploadLightingResources
		);

		void operator()() override;

	protected:
		/// <summary>
		/// Sort opaque game objects from near to far
		/// </summary>
		/// <returns>List of sorted opaque objects</returns>
		std::list<GameObject*> SortOpaque();

		void SetUpPipelineStates() override;

		std::function<void(void)> uploadShadowResources;
		std::function<void(void)> uploadLightingResources;
	};
}
