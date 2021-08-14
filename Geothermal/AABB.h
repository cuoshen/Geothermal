#pragma once
#include "pch.h"

/// <summary>
/// Axis-Aligned Bounding Box
/// </summary>
struct AABB
{
	DirectX::XMFLOAT3 MinXYZ;
	DirectX::XMFLOAT3 MaxXYZ;
};