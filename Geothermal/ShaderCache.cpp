#include "pch.h"
#include "ShaderCache.h"

using namespace Geothermal::Graphics;
using namespace Bindables;
using namespace std;

ShaderCache& ShaderCache::Instance()
{
	static ShaderCache instance;
	return instance;
}

VertexShader* ShaderCache::VertexShader
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

VertexShader* ShaderCache::VertexShader(std::wstring name)
{
	auto result = vertexShaderCache.find(name);
	if (result != vertexShaderCache.end())
	{
		return result->second.get();
	}
	// Return nullptr on cache miss
	return nullptr;
}

PixelShader* ShaderCache::PixelShader(wstring name)
{
	auto result = pixelShaderCache.find(name);
	if (result == pixelShaderCache.end())
	{
		return CompilePixelShader(name);
	}

	return result->second.get();
}

VertexShader* ShaderCache::CompileVertexShader
(
	wstring name, D3D11_INPUT_ELEMENT_DESC* inputSignatures, UINT inputElementCount
)
{

}

PixelShader* ShaderCache::CompilePixelShader(wstring name)
{

}
