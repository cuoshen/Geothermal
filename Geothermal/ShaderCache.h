#pragma once
#include <unordered_map>
#include "Shaders.h"

namespace Geothermal::Graphics
{
	/// <summary>
	/// A shader cache that compiles the 
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

		std::unordered_map<std::string, Bindables::VertexShader> vertexShaderCache;
		std::unordered_map<std::string, Bindables::PixelShader> pixelShaderCache;
	};
}
