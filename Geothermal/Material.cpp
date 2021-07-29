#include "pch.h"
#include "Shaders.h"
#include "ShaderCache.h"
#include "Material.h"

using namespace Geothermal::Graphics;
using namespace Materials;
using namespace Bindables;
using namespace std;

Material::Material
(
	wstring vertexShaderName, wstring pixelShaderName,
	 D3D11_INPUT_ELEMENT_DESC* inputSignatures, UINT inputElementCount
):
	vertexShaderName(vertexShaderName), pixelShaderName(pixelShaderName)
{
	// Precompile shaders
	(void)ShaderCache::Instance()->PixelShader(pixelShaderName);
	(void)ShaderCache::Instance()->VertexShader(vertexShaderName, inputSignatures, inputElementCount);
}

void Material::Bind()
{
	BindShadersAndParameters();
	BindTextures();
}

void Material::BindShadersAndParameters()
{
	ShaderCache::Instance()->PixelShader(pixelShaderName)->Bind();
	ShaderCache::Instance()->VertexShader(vertexShaderName)->Bind();

	for (auto parameterBuffer : parameters)
	{
		parameterBuffer->Bind();
	}
}

void Material::BindTextures()
{
	for (shared_ptr<Texture2D> texture : textures)
	{
		ID3D11ShaderResourceView* textureAsSRV = texture->UseAsShaderResource().get();
	}
}
