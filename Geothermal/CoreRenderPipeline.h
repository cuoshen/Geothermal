#pragma once
#include "DeviceResources.h"
#include "Camera.h"

//#define DEBUG_SHAPES	// TODO: put this entire test region into a debug scene

#ifdef DEBUG_SHAPES
#include "Mesh.h"
#include "ConstantBuffer.h"
#endif

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
	private:
#ifdef DEBUG_SHAPES
		void DrawDebugTriangle();
		void DrawDebugQuad();

		Meshes::Mesh* debugMesh;
		Graphics::Bindables::VertexConstantBuffer<XMMATRIX>* debugModelTransform;
		struct PhongAttributes
		{
			XMFLOAT4 Ambient;
			XMFLOAT4 BaseColor;
			float Diffuse;
			float Specular;
			float Smoothness;
			float Padding;	// Constant buffer size must be a multiple of 16 bytes, add empty padding
		};
		PhongAttributes shadingParameters;
		void LoadDebugMesh();
		void DrawDebugMesh();
#endif
		std::shared_ptr<DeviceResources> deviceResources;
		std::unique_ptr<Camera> camera;
	};
}