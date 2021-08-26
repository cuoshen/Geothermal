#pragma once
#include <functional>
#include "RenderPass.h"
#include "SceneGeometryPass.h"

namespace Geothermal::Graphics::Passes
{
	constexpr uint GBufferCount = 3;

	/// <summary>
	/// Deferred rendering geometry pass.
	/// Render all the objects in the frustum into a collection of GBuffers,
	/// which is later used in DeferredLightingPass to shade the actual scene.
	/// </summary>
	class DeferredGBufferPass : public SceneGeometryPass
	{
	public:
		/// <param name="sink">
		/// GBuffer layout: 
		/// [0] R8G8B8 Albedo A8 Unused;
		/// [1] R32G32B32 World Space Normal A32 Roughness;
		/// [2] R32 Depth;
		/// </param>
		DeferredGBufferPass
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			std::unique_ptr<std::vector<Texture2D*>> sink
		);

		void operator()() override;

	protected:
		void SetUpPipelineStates() override;
		ID3D11RenderTargetView* renderTargets[GBufferCount - 1];
		ID3D11DepthStencilView* depthStencil;
	};
}
