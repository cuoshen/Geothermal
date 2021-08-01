#include "pch.h"
#include "Shaders.h"
#include "ShaderCache.h"
#include "Material.h"

using namespace Geothermal::Graphics;
using namespace Materials;
using namespace Bindables;
using namespace Structures;
using namespace std;
using namespace winrt;

Material::Material
(
	wstring vertexShaderName, wstring pixelShaderName,
	 D3D11_INPUT_ELEMENT_DESC* inputSignatures, uint inputElementCount
):
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

void Material::Bind(shared_ptr<DeviceResources> const& deviceResources)
{
	BindShadersAndParameters(deviceResources);
	BindTextures(deviceResources);
}

void Material::BindShadersAndParameters(shared_ptr<DeviceResources> const& deviceResources)
{
	ShaderCache::Instance()->PixelShader(pixelShaderName)->Bind();
	ShaderCache::Instance()->VertexShader(vertexShaderName)->Bind();

	for (auto parameterBuffer : parameters)
	{
		parameterBuffer->Bind();
	}
}

void Material::BindTextures(shared_ptr<DeviceResources> const& deviceResources)
{
	for (shared_ptr<Texture2D> texture : textures)
	{
		ID3D11ShaderResourceView* srv = texture->UseAsShaderResource().get();
		deviceResources->Context()->PSSetShaderResources(texture->Slot(), 1, &srv);
	}
}
