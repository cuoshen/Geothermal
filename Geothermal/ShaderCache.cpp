#include "pch.h"
#include "ShaderCache.h"

using namespace Geothermal::Graphics;
using namespace Bindables;
using namespace std;

ShaderCache* ShaderCache::instance;

void ShaderCache::Initialize(shared_ptr<DeviceResources> const& deviceResources)
{
	assert(ShaderCache::instance == nullptr);
	ShaderCache::instance = new ShaderCache(deviceResources);
}

ShaderCache* ShaderCache::Instance()
{
	assert(ShaderCache::instance != nullptr);
	return ShaderCache::instance;
}

Bindables::VertexShader* ShaderCache::VertexShader
(
	wstring name, D3D11_INPUT_ELEMENT_DESC* inputSignatures, UINT inputElementCount
)
{
	auto result = vertexShaderCache.find(name);
	if (result == vertexShaderCache.end())
	{
		return CompileVertexShader(name, inputSignatures, inputElementCount);
	}

	return result->second.get();
}

Bindables::VertexShader* ShaderCache::VertexShader(std::wstring name)
{
	auto result = vertexShaderCache.find(name);
	if (result != vertexShaderCache.end())
	{
		return result->second.get();
	}
	// Return nullptr on cache miss
	return nullptr;
}

Bindables::PixelShader* ShaderCache::PixelShader(wstring name)
{
	auto result = pixelShaderCache.find(name);
	if (result == pixelShaderCache.end())
	{
		return CompilePixelShader(name);
	}

	return result->second.get();
}

ShaderCache::ShaderCache(shared_ptr<DeviceResources> const& deviceResources) :
	deviceResources(deviceResources)
{
}

Bindables::VertexShader* ShaderCache::CompileVertexShader
(
	wstring name, D3D11_INPUT_ELEMENT_DESC* inputSignatures, UINT inputElementCount
)
{
	unique_ptr<Bindables::VertexShader> compiledVertexShader =
		make_unique<Bindables::VertexShader>(deviceResources, name, inputSignatures, inputElementCount);
	Bindables::VertexShader* product = compiledVertexShader.get();

	vertexShaderCache.insert
	(
		std::make_pair
		(
			name,
			std::move(compiledVertexShader)
		)
	);

	return product;
}

Bindables::PixelShader* ShaderCache::CompilePixelShader(wstring name)
{
	unique_ptr<Bindables::PixelShader> compiledPixelShader =
		make_unique<Bindables::PixelShader>(deviceResources, name);
	Bindables::PixelShader* product = compiledPixelShader.get();

	pixelShaderCache.insert
	(
		std::make_pair
		(
			name,
			std::move(compiledPixelShader)
		)
	);

	return product;
}
