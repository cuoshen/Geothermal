#pragma once
#include <functional>
#include "GraphicResources.h"
#include "Camera.h"
#include "ViewPoint.h"
#include "RenderPasses.h"

namespace Geothermal::Graphics
{
	/// <summary>
	/// Global render pipeline object
	/// that is in charge of scheduling and dispatching rendering tasks.
	/// </summary>
	class CoreRenderPipeline
	{
	public:
		CoreRenderPipeline(std::shared_ptr<DeviceResources> const& deviceResources);

		/// <summary>
		/// Render the whole scene in a single forward pass with the main camera
		/// </summary>
		void Render();

		const Structures::DirectionalLight& MainLight() { return mainLight; }

	private:
		void StartGUIFrame();
		void DrawGUI();
		void ResetCamera();

		std::unique_ptr<Passes::ShadowPass> shadowPass;
		std::unique_ptr<Passes::SimpleForwardPass> simpleForwardPass;
		std::unique_ptr<Passes::PostProcessingPass> postProcessingPass;
		std::unique_ptr<Passes::WireframeDebugPass> debugPass;
		bool debugMode;

		std::unique_ptr<Texture2D> hdrTargets[4];

		Structures::DirectionalLight mainLight;
		// TODO: Refactor into dedicated shadow caster class
		const XMVECTORF32 mainLightShadowCastingOrigin = { 0.0f, 10.0f, 0.0f };
		std::shared_ptr<ShadowMap> mainShadowMap;
		XMMATRIX world2light;
		void UploadShadowResources();
		void UploadLightingResources();
		Bindables::VertexConstantBuffer<DirectX::XMMATRIX > ShadowCasterParametersBuffer;

		Structures::LightBuffer lights;
		std::unique_ptr<Bindables::PixelConstantBuffer<Structures::LightBuffer>> lightsConstantBuffer;
		std::unique_ptr<Camera> camera;

		std::shared_ptr<DeviceResources> deviceResources;
	};
}