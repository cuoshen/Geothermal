#pragma once
#include "DeviceResources.h"
#include "Bindable.h"
#include <d3dcompiler.h>

namespace Geothermal::Graphics::Bindables
{
	class VertexShader : public Bindable
	{
	public:
		VertexShader
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			std::wstring filename,
			D3D11_INPUT_ELEMENT_DESC* vertexLayout,
			UINT layoutElementCount
		) :
			Bindable(deviceResources)
		{
			// Use a blob to read compiled shaders
			winrt::com_ptr<ID3DBlob> blob = nullptr;		// The blob is used once, self destructs after the constructor
			winrt::check_hresult(
				D3DReadFileToBlob(filename.c_str(), blob.put())
			);
			// Create vertex shader
			winrt::check_hresult(
				deviceResources->Device()->
				CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, vertexShader.put())
			);
			winrt::check_hresult(
				deviceResources->Device()->
				CreateInputLayout
				(
					vertexLayout,
					layoutElementCount,
					blob->GetBufferPointer(),
					blob->GetBufferSize(),
					layout.put()
				)
			);
		}

		void Bind() override
		{
			deviceResources->Context()->VSSetShader(vertexShader.get(), 0, 0);
			deviceResources->Context()->IASetInputLayout(layout.get());
		}

	private:
		winrt::com_ptr<ID3D11VertexShader> vertexShader;
		winrt::com_ptr<ID3D11InputLayout> layout;	// Store the vertex layout as well, since it is vertex shader specific
	};
}
