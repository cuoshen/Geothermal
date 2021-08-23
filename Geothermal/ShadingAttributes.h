#pragma once
#include "pch.h"

namespace Geothermal::Graphics::Structures
{
	enum TEXTURE_FLAGS
	{
		USE_ALBEDO_MAP = 0x01,
		USE_NORMAL_MAP = 0x02,
		USE_SHADOW_MAP = 0x04
	};

	struct ShadingAttributes
	{
		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 BaseColor;
		float Diffuse;
		float Specular;
		float Smoothness;
		// In order to keep the struct 16-bytes-aligned, we combine texture flags into the same field
		int TextureFlags;
	};

	struct DirectionalLight
	{
		DirectX::XMFLOAT4		Color;
		DirectX::XMFLOAT3		Direction;
		float									Padding0;
	};

	struct Light
	{
		DirectX::XMFLOAT4		Color;
		DirectX::XMFLOAT4		Position;
		float									Radius;
		float									Attenuation;
		int										Type;
		float									Padding0;
	};

	constexpr uint maximumPointLightNumber = 32;

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
			memset(AdditionalLights, 0, sizeof(Light) * maximumPointLightNumber);
		}
		DirectionalLight			MainLight;
		Light								AdditionalLights[maximumPointLightNumber];
		DirectX::XMUINT4		LightActivation;
	};
}