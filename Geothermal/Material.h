#pragma once
#include "Shaders.h"
#include "ShaderCache.h"
#include <string>

namespace Geothermal::Graphics::Materials
{
	/// <summary>
	/// A material is an encapsulation of a collections of shaders of different types 
	/// as well as the associated parameters & resources.
	/// </summary>
	class Material
	{
	public:
		Material(std::string vertexShaderName, std::string pixelShaderName);
	protected:
		std::shared_ptr<ShaderCache> cache;
	};
}
