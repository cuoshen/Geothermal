#pragma once
#include "DeviceResources.h"
#include "Mesh.h"
#include "ConstantBuffer.h"

namespace Geothermal::Graphics
{
	/// <summary>
	/// Each drawable object carrys exactly one object renderer, which is in charge of issuing the draw call
	/// </summary>
	class ObjectRenderer
	{
	public:
		ObjectRenderer(Meshes::Mesh mesh, shared_ptr<DeviceResources> const& deviceResources);
		void Render(DirectX::XMMATRIX object2World);
		void BindAllResources();
	protected:
		std::shared_ptr<DeviceResources> deviceResources;
		Meshes::Mesh mesh;
		/// <summary>
		/// The transform constant buffer is bound to the vertex buffer
		/// Its payload is a matrix that transforms the vertex position from object space to model space
		/// </summary>
		Bindables::VertexConstantBuffer<XMMATRIX> transformBuffer;
	};
}
