#pragma once
#include "DeviceResources.h"
#include "Bindable.h"
#include <d3dcompiler.h>

using namespace std;

namespace Geothermal::Graphics::Bindables
{
	class PixelShader : public Bindable
	{
	public:
		PixelShader
		(
			shared_ptr<DeviceResources> const& deviceResources,
			wstring filename
		):
			Bindable(deviceResources)
		{
			// Use a blob to read compiled shaders
			winrt::com_ptr<ID3DBlob> blob = nullptr;		// The blob is used once, self destructs after the constructor
			winrt::check_hresult(
				D3DReadFileToBlob(filename.c_str(), blob.put())
			);
			winrt::check_hresult(
				deviceResources->GetD3DDevice()->
				CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, pixelShader.put())
			);
		}

		void Bind() override
		{
			deviceResources->GetD3DDeviceContext()->PSSetShader(pixelShader.get(), 0, 0);
		}

	private:
		winrt::com_ptr<ID3D11PixelShader> pixelShader;
	};
}
