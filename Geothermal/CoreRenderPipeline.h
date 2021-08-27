#pragma once
#include <functional>
#include "GraphicResources.h"
#include "Camera.h"
#include "ViewPoint.h"
#include "RenderPasses.h"

namespace Geothermal::Graphics
{
	constexpr uint HDRTargetCount = 4;

	/// <summary>
	/// Global render pipeline object
	/// that is in charge of scheduling and dispatching rendering tasks.
	/// </summary>
	class CoreRenderPipeline
	{
	public:
		CoreRenderPipeline(std::shared_ptr<DeviceResources> const& deviceResources);

		void Render();

		const Structures::DirectionalLight& MainLight() { return mainLight; }

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

		Structures::DirectionalLight mainLight;
		// TODO: Refactor into dedicated shadow caster class
		const XMVECTORF32 mainLightShadowCastingOrigin = { 0.0f, 10.0f, 0.0f };
		std::shared_ptr<ShadowMap> mainShadowMap;
		XMMATRIX world2light;
		void UploadShadowResources();
		void UploadLightingResources();
		Bindables::VertexConstantBuffer<DirectX::XMMATRIX > ShadowCasterParametersBuffer;

		Structures::ForwardLightBuffer lights;
		std::unique_ptr<Bindables::PixelConstantBuffer<Structures::ForwardLightBuffer>> lightsConstantBuffer;
		std::unique_ptr<Camera> camera;

		std::shared_ptr<DeviceResources> deviceResources;
	};
}