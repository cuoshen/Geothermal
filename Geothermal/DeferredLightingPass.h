#pragma once
#include <functional>
#include "RenderPass.h"

namespace Geothermal::Graphics::Passes
{
	/// <summary>
	/// Given the GBuffers and lighting info, draw the scene.
	/// </summary>
	class DeferredLightingPass : public RenderPass
	{
	public:
		/// <param name="sink">The GBuffer produced in DeferredGBufferPass</param>
		/// <param name="source">HDR output target</param>
		DeferredLightingPass
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			std::unique_ptr<std::vector<Texture2D*>> source,
			std::unique_ptr<std::vector<Texture2D*>> sink
		);

		void SetDelegates
		(
			std::function<void(void)> uploadShadowResources
		);

		void operator()() override;

	protected:
		void SetUpPipelineStates() override;

		/// <summary>
		/// Generate a full screen quad and shade everything with directional main light
		/// </summary>
		std::unique_ptr<Materials::Material> deferredDirectionalLit;

		std::unique_ptr<DirectX::CommonStates> states;
		std::function<void(void)> uploadShadowResources;
	};
}
