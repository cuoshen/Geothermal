#pragma once
#include "RenderPass.h"
#include "SceneGeometryPass.h"

namespace Geothermal::Graphics::Passes
{
	/// <summary>
	/// Deferred rendering geometry pass.
	/// </summary>
	class DeferredGBufferPass : SceneGeometryPass
	{
	public:
		/// <param name="sink">
		/// GBuffer layout: 
		/// [0] 
		/// </param>
		DeferredGBufferPass
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			std::unique_ptr<std::vector<Texture2D*>> sink
		);

		void operator()() override;
	};
}
