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

	private:
		std::unordered_map<std::string, Bindables::VertexShader> vertexShaderCache;
		std::unordered_map<std::string, Bindables::PixelShader> pixelShaderCache;
	};
}
