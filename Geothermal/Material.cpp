#include "pch.h"
#include "Material.h"

using namespace Geothermal::Graphics::Materials;
using namespace std;

Material::Material(string vertexShaderName, string pixelShaderName, bool precompileOnLoad):
	vertexShaderName(vertexShaderName), pixelShaderName(pixelShaderName)
{
	if (precompileOnLoad)
	{
		// Have the shader cache compile both shaders
	}
}
