#pragma once
#include "GraphicResources.h"
#include "Mesh.h"

namespace Geothermal::Graphics
{
	/// <summary>
	/// Affects sorting order of objects
	/// </summary>
	enum class ObjectRenderType
	{
		OPAQUE_OBJECT,
		TRANSPARENT_OBJECT
	};

	class ObjectRenderer
	{
	public:
		/// <summary>
		/// Each drawable object carrys exactly one object renderer, 
		/// which is in charge of issuing the draw call.
		/// </summary>
		/// <param name="mesh">Mesh data containing all vertices of the renderable</param>
		/// <param name="material">Active material assigned to the renderable, 
		/// can be null in which case no material info is sent to GPU.</param>
		ObjectRenderer
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			Meshes::Mesh mesh, std::unique_ptr<Materials::Material> material
		);
		void Render(DirectX::XMMATRIX object2World);
		void BindAllResources();
	protected:
		std::shared_ptr<DeviceResources> deviceResources;

		Meshes::Mesh mesh;
		/// <summary>
		/// A matrix that transforms the vertex position from object space to model space
		/// </summary>
		Bindables::VertexConstantBuffer<DirectX::XMMATRIX> object2WorldTransformBuffer;
		std::unique_ptr<Materials::Material> material;
	};
}
