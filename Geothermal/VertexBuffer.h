#pragma once
#include "DeviceResources.h"
#include "Bindable.h"

namespace Geothermal::Graphics::Bindables
{
	/// <summary>
	/// Bindable unindexed vertex buffer
	/// </summary>
	template<typename V>
	class VertexBuffer : public Bindable
	{
	public:
		/// <summary>
		/// Build vertex buffer from a vector of vertices
		/// </summary>
		VertexBuffer
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			const std::vector<V> vertices
		):
			Bindable(deviceResources),
			vertices(vertices), vertexBuffer(nullptr), stride(sizeof(V)), offset(0), vertexCount(vertices.size())
		{
			// Initialize vertex buffer
			D3D11_BUFFER_DESC bufferDescription = { 0 };
			bufferDescription.Usage = D3D11_USAGE_DEFAULT;
			bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			// Default CPU access and misc flags
			bufferDescription.ByteWidth = sizeof(V) * this->vertices.size();
			bufferDescription.StructureByteStride = sizeof(V);

			D3D11_SUBRESOURCE_DATA data = { 0 };
			data.pSysMem = this->vertices.data();

			winrt::check_hresult(
				deviceResources->D3DDevice()->CreateBuffer(&bufferDescription, &data, vertexBuffer.put())
			);
		}

		void Bind() override
		{
			ID3D11Buffer* vertexBufferPtr = this->vertexBuffer.get();
			deviceResources->D3DDeviceContext()->IASetVertexBuffers(0, 1, &vertexBufferPtr, &stride, &offset);
			deviceResources->D3DDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		virtual bool IsIndexed() 
		{
			return false;
		}

		UINT GetVertexCount() const
		{
			return vertexCount;
		}
	protected:
		const UINT offset;
		const UINT stride;
		UINT vertexCount;
		std::vector<V> vertices;
		winrt::com_ptr<ID3D11Buffer> vertexBuffer;
	};

	template<typename V>
	class IndexedVertexBuffer : public VertexBuffer<V>
	{
	public:
		/// <summary>
		/// If we don't supply the index buffer, we would just create an unindexed vertex buffer
		/// </summary>
		IndexedVertexBuffer
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			const std::vector<V> vertices
		):
			VertexBuffer<V>(deviceResources, vertices), 
			isIndexed(false), indexBuffer(nullptr), indices(), indexCount(0)
		{
		}

		/// <summary>
		/// Fully indexed vertex buffer
		/// </summary>
		IndexedVertexBuffer
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			const std::vector<V> vertices,
			const std::vector<UINT> indices
		) :
			VertexBuffer<V>(deviceResources, vertices), 
			isIndexed(true), indexBuffer(nullptr), indices(indices), indexCount(indices.size())
		{
			// Initialize index buffer
			D3D11_BUFFER_DESC indexBufferDescription = { 0 };
			indexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
			// Default CPU access and misc flags
			indexBufferDescription.ByteWidth = sizeof(UINT) * this->indices.size();
			indexBufferDescription.StructureByteStride = sizeof(UINT);

			D3D11_SUBRESOURCE_DATA data = { 0 };
			data.pSysMem = this->indices.data();

			winrt::check_hresult(
				deviceResources->D3DDevice()->CreateBuffer(&indexBufferDescription, &data, indexBuffer.put())
			);
		}

		using VertexBuffer<V>::deviceResources;

		void Bind() override
		{
			VertexBuffer<V>::Bind();
			if (isIndexed)
			{
				// Bind index buffer
				deviceResources->D3DDeviceContext()->IASetIndexBuffer(indexBuffer.get(), DXGI_FORMAT_R32_UINT, 0);
			}
		}

		bool IsIndexed() override
		{
			return isIndexed;
		}

		UINT GetIndexCount() const
		{
			assert(isIndexed);
			return indexCount;
		}
	private:
		bool isIndexed;
		UINT indexCount;
		std::vector<UINT> indices;
		winrt::com_ptr<ID3D11Buffer> indexBuffer;
	};
}