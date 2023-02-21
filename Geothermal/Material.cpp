#include "pch.h"
#include "Material.h"
#include "Shaders.h"
#include "ShaderCache.h"

using namespace Geothermal::Graphics;
using namespace Materials;
using namespace Bindables;
using namespace Structures;
using namespace std;
using namespace winrt;

Material::Material
(
	std::shared_ptr<DeviceResources> const& deviceResources,
	wstring vertexShaderName, wstring pixelShaderName,
	D3D11_INPUT_ELEMENT_DESC* inputSignatures, u32 inputElementCount
) :
	deviceResources(deviceResources),
	vertexShaderName(vertexShaderName), pixelShaderName(pixelShaderName)
{
	// Precompile shaders
	(void)ShaderCache::Instance()->PixelShader(pixelShaderName);
	(void)ShaderCache::Instance()->VertexShader(vertexShaderName, inputSignatures, inputElementCount);
}

void Material::AddParameterSet(shared_ptr<ConstantBuffer<ShadingAttributes>> parameterSet)
{
	parameters.push_back(parameterSet);
}

void Material::AddTexture(shared_ptr<Texture2D> texture)
{
	textures.push_back(texture);
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
		ID3D11ShaderResourceView* srv = texture->UseAsShaderResource().get();
		deviceResources->Context()->PSSetShaderResources(texture->Slot(), 1, &srv);
	}
}
