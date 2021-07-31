#pragma once
#include <unordered_map>
#include "GraphicResources.h"

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
		static void Initialize(std::shared_ptr<DeviceResources> const& deviceResources);
		static ShaderCache* Instance();

		/// <summary>
		/// Returns a vertex shader if it is already in the registry, 
		/// otherwise try to compile it from DirectX Bytecode in cso file
		/// </summary>
		Bindables::VertexShader* VertexShader
		(
			std::wstring name, D3D11_INPUT_ELEMENT_DESC* inputSignatures, UINT inputElementCount
		);

		/// <summary>
		/// Returns a vertex shader if it is already in the registry,
		/// DO NOT RECOMPILE, return nullptr on cache miss
		/// </summary>
		Bindables::VertexShader* VertexShader(std::wstring name);

		/// <summary>
		/// Returns a pixel shader if it is already in the registry, 
		/// otherwise try to compile it from DirectX Bytecode in cso file
		/// </summary>
		Bindables::PixelShader* PixelShader(std::wstring name);

		Bindables::VertexShader* CompileVertexShader
		(
			std::wstring name, D3D11_INPUT_ELEMENT_DESC* inputSignatures, UINT inputElementCount
		);
		Bindables::PixelShader* CompilePixelShader(std::wstring name);

	private:
		static ShaderCache* instance;

		ShaderCache(std::shared_ptr<DeviceResources> const& deviceResources);
		std::shared_ptr<DeviceResources> deviceResources;

		// ShaderCache own all the shaders, thus the unique_ptr in vector

		std::unordered_map<std::wstring, std::unique_ptr<Bindables::VertexShader>> vertexShaderCache;
		std::unordered_map<std::wstring, std::unique_ptr<Bindables::PixelShader>> pixelShaderCache;
	};
}
