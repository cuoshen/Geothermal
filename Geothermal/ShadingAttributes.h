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

	struct DirectionalLight
	{
		DirectX::XMFLOAT4		Color;
		DirectX::XMFLOAT3		Direction;
		float								Padding0;
	};

	struct Light
	{
		DirectX::XMFLOAT4		Color;
		DirectX::XMFLOAT4		Position;
		float								Radius;
		float								Attenuation;
		int									Type;
		float								Padding0;
	};

	#define MAX_POINT_LIGHTS_IN_SCENE 32

	/// <summary>
	/// Pack all lighting information into this buffer
	/// then upload to GPU
	/// </summary>
	struct LightBuffer
	{
		/// <summary>
		/// Initialize with no additional lights
		/// </summary>
		LightBuffer(DirectionalLight main)
		{
			MainLight = main;
			LightActivation.x = 0;
		}
		DirectionalLight			MainLight;
		Light								AdditionalLights[MAX_POINT_LIGHTS_IN_SCENE];
		DirectX::XMUINT4		LightActivation;
	};
}