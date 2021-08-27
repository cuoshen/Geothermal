#pragma once
#include "pch.h"
#include "Texture2D.h"

namespace Geothermal::Graphics
{
	constexpr uint shadowMapSlot = 5u;

	/// <summary>
	/// A shadow map is a depth stencil | shader resource texture.
	/// It should be null initialized since we typically write to it every frame.
	/// </summary>
	class ShadowMap : public Texture2D
	{
	public:
		/// <summary>
		/// Geothermal uses a D32 depth map for shadows
		/// </summary>
		ShadowMap(std::shared_ptr<DeviceResources> const& deviceResources, UINT width, UINT height) :
			Texture2D
			(
				deviceResources, DXGI_FORMAT_R32_TYPELESS, width, height,
				D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL, shadowMapSlot
			)
		{
		}
	};
}
