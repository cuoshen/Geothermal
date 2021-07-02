#pragma once
#include "DeviceResources.h"
#include "Bindable.h"

namespace Geothermal::Graphics::Bindables
{
	/// <summary>
	/// The ConstantBuffer class is itself abstract, 
	/// we need to specify whether to bind buffer to vertex or pixel shader.
	/// </summary>
	/// <typeparam name="C">Constant Type to be bound</typeparam>
	template<typename C>
	class ConstantBuffer : public Bindable
	{
	public:
		/// <summary>
		/// Create constant buffer from data
		/// </summary>
		ConstantBuffer
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			const C& constant, UINT slot = 0u
		):
			Bindable(deviceResources),
			slot(slot)
		{
			D3D11_BUFFER_DESC constantBufferDescription = { 0 };
			constantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
			constantBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constantBufferDescription.ByteWidth = sizeof(C);
			constantBufferDescription.StructureByteStride = 0u;

			D3D11_SUBRESOURCE_DATA data = { 0 };
			data.pSysMem = &constant;
			winrt::check_hresult(
				deviceResources->D3DDevice()->CreateBuffer(&constantBufferDescription, &data, constantBuffer.put())
			);
		}

		/// <summary>
		/// Create empty constant buffer
		/// </summary>
		ConstantBuffer
		(
			std::shared_ptr<DeviceResources> const& deviceResources, UINT slot = 0u
		):
			Bindable(deviceResources),
			slot(slot)
		{
			D3D11_BUFFER_DESC constantBufferDescription = { 0 };
			constantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
			constantBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constantBufferDescription.ByteWidth = sizeof(C);
			constantBufferDescription.StructureByteStride = 0u;

			winrt::check_hresult(
				deviceResources->D3DDevice()->CreateBuffer(&constantBufferDescription, nullptr, constantBuffer.put())
			);
		}

		/// <summary>
		/// Update the payload in the constant buffer
		/// </summary>
		void Update(const C& newConstant)
		{
			D3D11_MAPPED_SUBRESOURCE subResource;
			deviceResources->D3DDeviceContext()->
				Map(constantBuffer.get(), 0u, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
			memcpy(subResource.pData, &newConstant, sizeof(newConstant));
			deviceResources->D3DDeviceContext()->
				Unmap(constantBuffer.get(), 0u);
		}

	protected:
		winrt::com_ptr<ID3D11Buffer> constantBuffer;
		UINT slot;
	};

	template<typename C>
	class VertexConstantBuffer : public ConstantBuffer<C>
	{
	public:
		using ConstantBuffer<C>::ConstantBuffer;
		using ConstantBuffer<C>::constantBuffer;
		using ConstantBuffer<C>::slot;
		using ConstantBuffer<C>::deviceResources;

		void Bind() override
		{
			ID3D11Buffer* cbuffer = constantBuffer.get();
			deviceResources->D3DDeviceContext()->VSSetConstantBuffers(slot, 1u, &cbuffer);
		}
	};

	template<typename C>
	class PixelConstantBuffer : public ConstantBuffer<C>
	{
	public:
		using ConstantBuffer<C>::ConstantBuffer;
		using ConstantBuffer<C>::constantBuffer;
		using ConstantBuffer<C>::slot;
		using ConstantBuffer<C>::deviceResources;

		void Bind() override
		{
			ID3D11Buffer* cbuffer = constantBuffer.get();
			deviceResources->D3DDeviceContext()->PSSetConstantBuffers(slot, 1u, &cbuffer);
		}
	};
}