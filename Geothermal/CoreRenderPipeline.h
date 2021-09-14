#pragma once
#include <functional>
#include "GraphicResources.h"
#include "RenderPipeline.h"
#include "Camera.h"
#include "ViewPoint.h"
#include "RenderPasses.h"

namespace Geothermal::Graphics
{
	constexpr uint HDRTargetCount = 4;
	constexpr bool DeferredMode = true;

	/// <summary>
	/// Default render pipeline implementation in Geothermal engine.
	/// </summary>
	class CoreRenderPipeline : public RenderPipeline
	{
	public:
		CoreRenderPipeline(std::shared_ptr<DeviceResources> const& deviceResources);
		void Render() override;

		const Structures::DirectionalLight& MainLight() { return lights.MainLight; }

	private:
		void StartGUIFrame();
		void DrawGUI();
		void ResetCamera();

		void InitializeHDRTargets();
		void InitializeGBuffers();
		void BuildRenderGraph();

		std::unique_ptr<Passes::ShadowPass> shadowPass;
		std::unique_ptr<Passes::SimpleForwardPass> simpleForwardPass;
		std::unique_ptr<Passes::DeferredGBufferPass> deferredGBufferPass;
		std::unique_ptr<Passes::DeferredLightingPass> deferredLightingPass;
		std::unique_ptr<Passes::PostProcessingPass> postProcessingPass;
		std::unique_ptr<Passes::WireframeDebugPass> debugPass;
		bool debugMode;

		std::unique_ptr<Texture2D> hdrTargets[HDRTargetCount];
		std::unique_ptr<Texture2D> gBuffers[Passes::GBufferCount];

		// TODO: Refactor into dedicated shadow caster class
		const XMVECTORF32 mainLightShadowCastingOrigin = { 0.0f, 10.0f, 0.0f };
		std::shared_ptr<ShadowMap> mainShadowMap;
		XMMATRIX world2light;
		void UploadShadowResources();
		void UploadLightingResources();
		Bindables::VertexConstantBuffer<DirectX::XMMATRIX > shadowParametersVSBuffer;
		Bindables::PixelConstantBuffer<DirectX::XMMATRIX > shadowParametersPSBuffer;

		Structures::ForwardLightBuffer lights;
		std::unique_ptr<Bindables::PixelConstantBuffer<Structures::ForwardLightBuffer>> lightsConstantBuffer;
		std::unique_ptr<Camera> camera;

		const DirectX::XMFLOAT3 deferredAmbience = { 0.0f, 0.0f, 0.06f };
	};
}