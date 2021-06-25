#pragma once
#include "Shaders.h"
#include "ShaderCache.h"
#include <string>

namespace Geothermal::Graphics::Materials
{
	class Material
	{
	public:
		Material();
	protected:
		std::shared_ptr<ShaderCache> cache;
		std::string vertexShaderKey;
		std::string pixelShaderKey;
	};
}
