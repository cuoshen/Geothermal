#pragma once
#include "pch.h"

// AABB is used extensively outside of just the graphics module
// thus it belongs here with the rest of the ultilities

namespace Geothermal
{
	/// <summary>
	/// Axis-Aligned Bounding Box
	/// </summary>
	struct AABB
	{
		DirectX::XMFLOAT3 MinXYZ;
		DirectX::XMFLOAT3 MaxXYZ;
	};

	/// <summary>
	/// Generate all bounding box vertices from 2-points input
	/// </summary>
	/// <returns>All 6 box vertices in model space</returns>
	std::array<DirectX::XMFLOAT3, 6> GenerateBoxVertices(AABB box);

	/// <summary>
	/// Generate all bounding box vertices from 2-points input, 
	/// transformed into another coordinate space.
	/// </summary>
	/// <returns>All 6 box vertices in desirable coordinate space</returns>
	std::array<DirectX::XMFLOAT3, 6> GenerateBoxVertices(AABB box, DirectX::XMMATRIX transform);
}
