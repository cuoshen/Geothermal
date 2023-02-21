#pragma once
#include "DeviceResources.h"
#include "GeothermalTypes.h"

namespace Geothermal::Graphics
{
	// TODO: find a subnamespace to put this module

	enum class TEXTURE_FILE_TYPE
	{
		DDS,
		PNG
	};

	/// <summary>
	/// 2D texture object with can be accessed via many views
	/// </summary>
	class Texture2D
	{
		// To represent the fact that an underlying texture can be used as 
		// many different views, we do not make this class into a Bindable 

	public:
		/// <summary>
		/// Create texture from a file of a supported type
		/// </summary>
		Texture2D(std::shared_ptr<DeviceResources> const& deviceResources,
			winrt::hstring const& filename, TEXTURE_FILE_TYPE fileType, u32 slot);

		/// <summary>
		/// For non-DDS image file loaded into the memory, we need to pass in metadata
		/// </summary>
		Texture2D(std::shared_ptr<DeviceResources> const& deviceResources, std::vector<char> data,
			DXGI_FORMAT format, u32 width, u32 height, u32 bitsPerPixel, u32 bindFlags, u32 slot);

		/// <summary>
		/// Build one from scratch in given dimensions
		/// </summary>
		Texture2D(std::shared_ptr<DeviceResources> const& deviceResources,
			DXGI_FORMAT format, u32 width, u32 height, u32 bindFlags, u32 slot);

		winrt::com_ptr<ID3D11ShaderResourceView> UseAsShaderResource();
		winrt::com_ptr<ID3D11RenderTargetView> UseAsRenderTarget();
		winrt::com_ptr<ID3D11DepthStencilView> UseAsDepthStencil();

		u32 Slot() const
		{
			return slot;
		}

	protected:
		void CreateTextureFromMemory
		(
			std::vector<char> data, u32 width, u32 height, u32 bitsPerPixel, u32 bingFlags
		);
		void CreateShaderResourceView();
		void CreateRenderTargetView();
		void CreateDepthStencilView();

		D3D11_TEXTURE2D_DESC DefaultDescriptionFromParameters
		(
			u32 width, u32 height, u32 bindFlags
		);

		bool IsValidBindFlags(u32 bindFlags);

		std::shared_ptr<DeviceResources> deviceResources;

		winrt::com_ptr<ID3D11Texture2D> texture;
		DXGI_FORMAT format;
		u32 bindFlags;
		u32 slot;
		winrt::com_ptr<ID3D11ShaderResourceView> shaderResourceView;
		winrt::com_ptr<ID3D11RenderTargetView> renderTargetView;
		winrt::com_ptr<ID3D11DepthStencilView> depthStencilView;
	};
}
