#pragma once
#include "ConstantBuffer.h"
#include "DeviceResources.h"

namespace Geothermal::Graphics
{
	/// <summary>
	/// View parameters constant buffer structure layout
	/// </summary>
	struct ViewParameters
	{
		DirectX::XMMATRIX World2ClipTransform;
		DirectX::XMFLOAT3 CameraWorldPosition;
	};

	class ViewPoint
	{
	public:
		/// <summary>
		/// Orthographic viewpoint
		/// </summary>
		ViewPoint(std::shared_ptr<DeviceResources> const& deviceResources, 
			float viewWidth, float viewHeight, float nearZ, float farZ);

		/// <summary>
		/// Perspective viewpoint with a fixed POV XM_PIDIV4
		/// </summary>
		ViewPoint(std::shared_ptr<DeviceResources> const& deviceResources,
			float aspectRatio, float nearZ, float farZ);

	protected:
		void Bind(DirectX::XMMATRIX world2View);
		DirectX::XMMATRIX const& Perspective() { return perspective; }

		float aspectRatio;
		float nearZ;
		float farZ;

	private:
		DirectX::XMMATRIX perspective;
		ViewParameters parameters;

		void InitializeConstantBuffers(std::shared_ptr<DeviceResources> const& deviceResources);

		std::unique_ptr<Graphics::Bindables::VertexConstantBuffer<ViewParameters>> parametersBufferVS;
		std::unique_ptr<Graphics::Bindables::PixelConstantBuffer<ViewParameters>> parametersBufferPS;
	};
}