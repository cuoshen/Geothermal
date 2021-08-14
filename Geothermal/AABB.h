#pragma once
#include "pch.h"

// AABB is used extensively outside of just the graphics module
// thus it belongs here with the rest of the ultilities

/// <summary>
/// Axis-Aligned Bounding Box
/// </summary>
struct AABB
{
	DirectX::XMFLOAT3 MinXYZ;
	DirectX::XMFLOAT3 MaxXYZ;
};
