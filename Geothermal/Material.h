#pragma once

#include <vector>
#include <string>
#include "GraphicResources.h"

namespace Geothermal::Graphics::Materials
{
	/// <summary>
	/// A material is an encapsulation of a collections of shaders of different types 
	/// as well as the associated parameters & resources.
	/// </summary>
	class Material
	{
	public:
		Material
		(
			std::string vertexShaderName, std::string pixelShaderName,
			D3D11_INPUT_ELEMENT_DESC* inputSignatures, UINT inputElementCount
		);

		/// <summary>
		/// Bind the entire material, that is, ALL shaders, parameters and textures
		/// </summary>
		void Bind();
		void BindShadersAndParameters();
		void BindTextures();

	protected:
		std::string vertexShaderName;
		std::string pixelShaderName;
		std::vector<Bindables::ConstantBuffer<Structures::ShadingAttributes>*> parameters;
		std::vector<std::shared_ptr<Texture2D>> textures;
	};
}
