#pragma once
#include "DeviceResources.h"
#include "Bindable.h"

namespace Geothermal::Graphics::Bindables
{
	class SamplerState : public Bindable
	{
	public:
		SamplerState(std::shared_ptr<DeviceResources> const& deviceResources) :
			Bindable(deviceResources), samplerState(nullptr)
		{
			D3D11_SAMPLER_DESC samplerDescription = {};
			samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDescription.MinLOD = 0;
			samplerDescription.MaxLOD = FLT_MAX;

			winrt::check_hresult(
				deviceResources->Device()->
				CreateSamplerState(&samplerDescription, samplerState.put())
			);
		}

		void Bind() override
		{
			// For a very preliminary implementation we should simply bind to PS
			ID3D11SamplerState* samplerAddress = samplerState.get();
			deviceResources->Context()->PSSetSamplers(0, 1, &samplerAddress);
		}

	private:
		winrt::com_ptr<ID3D11SamplerState> samplerState;
	};
}
