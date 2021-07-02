#pragma once
#include "DeviceResources.h"
#include "Bindable.h"

namespace Geothermal::Graphics::Bindables
{
	class SamplerState : public Bindable
	{
	public:
		SamplerState(std::shared_ptr<DeviceResources> const& deviceResources):
			Bindable(deviceResources)
		{
			D3D11_SAMPLER_DESC samplerDescription = {};
			samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;

			winrt::check_hresult(
				deviceResources->D3DDevice()->
				CreateSamplerState(&samplerDescription, samplerState.put())
			);
		}

		void Bind() override
		{
			// For a very preliminary implementation we should simply bind to PS
			deviceResources->D3DDeviceContext()->PSSetSamplers(0, 1, samplerState.put());
		}
	private:
		winrt::com_ptr<ID3D11SamplerState> samplerState;
	};
}
