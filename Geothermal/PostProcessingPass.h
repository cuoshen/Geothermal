#pragma once
#include "RenderPass.h"
#include "PostProcess.h"

namespace Geothermal::Graphics::Passes
{
	class PostProcessingPass : public RenderPass
	{
	public:
		/// <summary>
		/// Apply bloom if needed, then apply a Reinhard tone mapping.
		/// </summary>
		/// <param name="source">[0]: HDR scene buffer from renderer</param>
		/// <param name="sink">[0,1]: Bloom textures, [2]: final HDR buffer</param>
		PostProcessingPass
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			std::vector<Texture2D*> const& source,
			std::vector<Texture2D*> const& sink
		);

		void operator()() override;

	protected:
		// TODO: Implement a set of my own post processors and remove DirectXTK implementations
		void ApplyBloom();

		std::unique_ptr<DirectX::BasicPostProcess> basicPostProcess;
		std::unique_ptr<DirectX::DualPostProcess> dualPostProcess;
		std::unique_ptr<DirectX::ToneMapPostProcess> toneMapper;

		float exposure;
		bool useBloom;
		float bloomSize;
		float bloomThreshold;
		float bloomBrightness;

		void SetUpPipelineStates() override;
	};
}
