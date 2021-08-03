#pragma once
#include <functional>
#include "GraphicResources.h"
#include "Camera.h"
#include "ViewPoint.h"

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

		/// <summary>
		/// Get a shadow map by rendering from the main light
		/// </summary>
		void ShadowPass();
		/// <summary>
		/// Draw every drawable geometry in a single forward pass
		/// </summary>
		void SimpleForwardPass();
		/// <summary>
		/// Remap HDR tone to SDR display, apply bloom and other effects accordingly
		/// </summary>
		void PostProcessingPass();

		std::list<std::function<void()>> linearRenderGraph;

		Structures::DirectionalLight mainLight;

		// TODO: Refactor into dedicated shadow caster class
		const XMUINT2 shadowMapDimensions = { 4096, 4096 };
		const XMFLOAT3 mainLightShadowCastingOrigin = { 0.0f, 10.0f, 0.0f };
		ShadowMap mainShadowMap;
		D3D11_VIEWPORT shadowViewPort;
		ViewPoint shadowCaster;
		XMMATRIX world2light;
		void UpdateWorld2Light();

		void UploadShadowResources();
		Bindables::VertexConstantBuffer<DirectX::XMMATRIX > parametersBufferVS;

		Structures::LightBuffer lights;
		std::unique_ptr<Bindables::PixelConstantBuffer<Structures::LightBuffer>> lightConstantBuffer;
		std::unique_ptr<Camera> camera;

		std::shared_ptr<DeviceResources> deviceResources;
	};
}