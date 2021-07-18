#pragma once
#include "DeviceResources.h"
#include "Camera.h"
#include "ShadingAttributes.h"
#include "ConstantBuffer.h"
#include "Texture2D.h"
#include "ShadowMap.h"
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

		void LoadAllShaders();
		/// <summary>
		/// Render the whole scene in a single forward pass with the main camera
		/// </summary>
		void Render();
		Structures::DirectionalLight& MainLight() { return mainLight; }
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

		Structures::DirectionalLight mainLight;

		const XMUINT2 shadowMapDimensions = { 4096, 4096 };
		const XMFLOAT3 mainLightShadowCastingOrigin = {0.0f, 10.0f, 0.0f};
		ShadowMap mainShadowMap;
		D3D11_VIEWPORT shadowViewPort;
		ViewPoint shadowCaster;
		XMMATRIX world2light;
		void UpdateWorld2Light();

		Structures::LightBuffer lights;
		std::unique_ptr<Bindables::PixelConstantBuffer<Structures::LightBuffer>> lightConstantBuffer;
		std::unique_ptr<Camera> camera;

		std::shared_ptr<DeviceResources> deviceResources;
	};
}