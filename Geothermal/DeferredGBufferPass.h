#pragma once
#include "RenderPass.h"
#include "SceneGeometryPass.h"

namespace Geothermal::Graphics::Passes
{
	class DeferredGBufferPass : SceneGeometryPass
	{
	public:
		DeferredGBufferPass
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			std::vector<Texture2D*> const& source,
			std::vector<Texture2D*> const& sink
		);

		void operator()() override;
	};
}
