#pragma once
#include <unordered_map>
#include "Shaders.h"

namespace Geothermal::Graphics
{
	/// <summary>
	/// A shader cache that compiles and stores all the shader in the application
	/// For now we reference shader via a uniform resource locator string 
	/// which is simply the name of the shader
	/// </summary>
	class ShaderCache
	{
	public:
		static ShaderCache& Instance();

		/// <summary>
		/// Returns a vertex shader if it is already in the registry, 
		/// otherwise try to compile it from DirectX Bytecode in cso file
		/// </summary>
		Bindables::VertexShader* VertexShader
		(
			std::string name, D3D11_INPUT_ELEMENT_DESC* inputSignatures, UINT inputElementCount
		);

		/// <summary>
		/// Returns a vertex shader if it is already in the registry,
		/// DO NOT RECOMPILE, return nullptr on cache miss
		/// </summary>
		Bindables::VertexShader* VertexShader(std::string name);

		/// <summary>
		/// Returns a pixel shader if it is already in the registry, 
		/// otherwise try to compile it from DirectX Bytecode in cso file
		/// </summary>
		Bindables::PixelShader* PixelShader(std::string name);

	private:
		Bindables::VertexShader* CompileVertexShader
		(
			std::string name, D3D11_INPUT_ELEMENT_DESC* inputSignatures, UINT inputElementCount
		);
		Bindables::PixelShader* CompilePixelShader(std::string name);

		// ShaderCache own all the shaders, thus the unique_ptr in vector

		std::unordered_map<std::string, std::unique_ptr<Bindables::VertexShader>> vertexShaderCache;
		std::unordered_map<std::string, std::unique_ptr<Bindables::PixelShader>> pixelShaderCache;
	};
}
