#include "pch.h"
#include "Shaders.h"
#include "ShaderCache.h"
#include "Material.h"

using namespace Geothermal::Graphics::Materials;
using namespace std;

Material::Material
(
	string vertexShaderName, string pixelShaderName, bool precompileOnLoad,
	D3D11_INPUT_ELEMENT_DESC* inputSignatures, UINT inputElementCount
):
	vertexShaderName(vertexShaderName), pixelShaderName(pixelShaderName)
{
	if (precompileOnLoad)
	{
		// Have the shader cache compile both shaders
	}
}

void Material::Bind()
{
	BindShadersAndParameters();
	BindTextures();
}

void Material::BindShadersAndParameters()
{
	ShaderCache::Instance().PixelShader(pixelShaderName)->Bind();
	ShaderCache::Instance().VertexShader(vertexShaderName)->Bind();

	for (auto parameterBuffer : parameters)
	{
		parameterBuffer.Bind();
	}
}

void Material::BindTextures()
{
}
