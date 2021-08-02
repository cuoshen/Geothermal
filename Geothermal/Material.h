#pragma once

#include <vector>
#include <string>
#include "GeothermalTypes.h"
#include "DeviceResources.h"
#include "Texture2D.h"
#include "ConstantBuffer.h"
#include "ShadingAttributes.h"

namespace Geothermal::Graphics::Materials
{
	/// <summary>
	/// A material is an encapsulation of a collections of shaders of different types 
	/// as well as the associated parameters & resources.
	/// </summary>
	class Material
	{
	public:
		/// <summary>
		/// Initialize material in a minimalistic fashion: get both shaders, worry about parameters later
		/// </summary>
		/// <param name="inputSignatures">A description of per-vertex information fed to vertex shader</param>
		Material
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			std::wstring vertexShaderName, std::wstring pixelShaderName,
			D3D11_INPUT_ELEMENT_DESC* inputSignatures, uint inputElementCount
		);

		void AddParameterSet(std::shared_ptr<Bindables::ConstantBuffer<Structures::ShadingAttributes>> parameterSet);

		void AddTexture(std::shared_ptr<Texture2D> texture);

		/// <summary>
		/// Bind the entire material, that is, ALL shaders, parameters and textures
		/// </summary>
		void Bind();
		void BindShadersAndParameters();
		void BindTextures();

	protected:
		std::wstring vertexShaderName;
		std::wstring pixelShaderName;
		std::vector
			<
			std::shared_ptr
			<
			Bindables::ConstantBuffer<Structures::ShadingAttributes>
			>
			>
			parameters;

		std::vector<std::shared_ptr<Texture2D>> textures;
		std::shared_ptr<DeviceResources> deviceResources;
	};
}
