#pragma once
#include "pch.h"
#include "DeviceResources.h"

// AABB is used extensively outside of just the graphics module
// thus it belongs here with the rest of the ultilities

namespace Geothermal
{
	constexpr u32 BoxVerticesCount = 8;

	/// <summary>
	/// Axis-Aligned Bounding Box
	/// </summary>
	struct AABB
	{
		DirectX::XMFLOAT3 MinXYZ;
		DirectX::XMFLOAT3 MaxXYZ;

		void UpdateBounds(DirectX::XMFLOAT3 point);

		/// <summary>
		/// Generate all bounding box vertices from 2-points input
		/// </summary>
		/// <returns>All 8 box vertices in model space</returns>
		std::array<DirectX::XMFLOAT3, BoxVerticesCount> GenerateBoxVertices();
		/// <summary>
		/// Generate all bounding box vertices from 2-points input, 
		/// transformed into another coordinate space.
		/// </summary>
		/// <returns>All 8 box vertices in desirable coordinate space</returns>
		std::array<DirectX::XMFLOAT4, BoxVerticesCount> GenerateBoxVertices(DirectX::XMMATRIX transform);

		void DrawWireFrame
		(
			std::shared_ptr<Graphics::DeviceResources> deviceResources, DirectX::XMMATRIX transform
		);
	};
}
