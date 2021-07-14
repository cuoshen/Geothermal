#pragma once
#include "pch.h"

namespace Geothermal::Graphics::Structures
{
	enum TEXTURE_FLAGS
	{
		USE_ALBEDO_MAP = 0x01,
		USE_NORMAL_MAP = 0x02,
	};

	struct PhongAttributes
	{
		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 BaseColor;
		float Diffuse;
		float Specular;
		float Smoothness;
		// In order to keep us aligned 
		// we combine UseAlbedoMap and UserNormalMap into the same field
		//	&0x01 bit for albedo, &0x02 bit for normal
		int TextureFlags;
	};
}