#pragma once
#include <functional>
#include "RenderPass.h"
#include "SceneGeometryPass.h"

namespace Geothermal::Graphics::Passes
{
	constexpr uint GBufferCount = 2;

	/// <summary>
	/// Deferred rendering geometry pass.
	/// </summary>
	class DeferredGBufferPass : SceneGeometryPass
	{
	public:
		/// <param name="sink">
		/// GBuffer layout: 
		/// [0] R8G8B8 Albedo A8 Unused;
		/// [1] R32G32B32 World Space Normal A32 Roughness;
		/// </param>
		DeferredGBufferPass
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			std::unique_ptr<std::vector<Texture2D*>> sink
		);

		void operator()() override;

	protected:
		void SetUpPipelineStates() override;
		ID3D11RenderTargetView* targets[GBufferCount];
	};
}
