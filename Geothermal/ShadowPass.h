#pragma once
#include "RenderPass.h"
#include "ViewPoint.h"

namespace Geothermal::Graphics::Passes
{
	class ShadowPass : public RenderPass
	{
	public:
		ShadowPass
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			std::vector<Texture2D*> const& source,
			std::vector<Texture2D*> const& sink
		);

		DirectX::XMMATRIX UpdateWorld2Light(DirectX::XMVECTOR castingOrigin, DirectX::XMVECTOR lightDirection);
		DirectX::XMMATRIX CasterPerspective();
		std::shared_ptr<ShadowMap> MainShadowMap() { return mainShadowMap; }

		void operator()() override;

	protected:
		const DirectX::XMUINT2 shadowMapDimensions = { 4096, 4096 };

		void SetUpPipelineStates() override;

		/// <summary>
		/// The shadow pass owns the main shadow map resource
		/// </summary>
		std::shared_ptr<ShadowMap> mainShadowMap;
		D3D11_VIEWPORT shadowViewPort;
		DirectX::XMMATRIX world2light;
		ViewPoint shadowCaster;
	};
}