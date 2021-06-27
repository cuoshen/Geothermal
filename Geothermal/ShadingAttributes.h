#pragma once
#include "pch.h"

namespace Geothermal::Graphics::Structures
{
	struct PhongAttributes
	{
		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 BaseColor;
		float Diffuse;
		float Specular;
		float Smoothness;
		float Padding;	// Constant buffer size must be a multiple of 16 bytes, add empty padding
	};
}