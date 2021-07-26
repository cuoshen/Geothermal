#pragma once
#include "DeviceResources.h"

namespace Geothermal::Graphics
{
	// TODO: find a subnamespace to put this module

	enum TEXTURE_FILE_TYPE
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
			winrt::hstring const& filename, TEXTURE_FILE_TYPE fileType);

		/// <summary>
		/// For non-DDS image file loaded into the memory, we need to pass in metadata
		/// </summary>
		Texture2D(std::shared_ptr<DeviceResources> const& deviceResources, std::vector<char> data, 
			DXGI_FORMAT format, UINT width, UINT height, UINT bitsPerPixel, UINT bindFlags);

		/// <summary>
		/// Build one from scratch
		/// </summary>
		Texture2D(std::shared_ptr<DeviceResources> const& deviceResources,
			DXGI_FORMAT format, UINT width, UINT height, UINT bindFlags);

		winrt::com_ptr<ID3D11ShaderResourceView> UseAsShaderResource();
		winrt::com_ptr<ID3D11RenderTargetView> UseAsRenderTarget();
		winrt::com_ptr<ID3D11DepthStencilView> UseAsDepthStencil();
	private:
		void CreateTextureFromMemory
		(
			std::vector<char> data, UINT width, UINT height, UINT bitsPerPixel, UINT bingFlags
		);
		void CreateShaderResourceView();
		void CreateRenderTargetView();
		void CreateDepthStencilView();

		D3D11_TEXTURE2D_DESC DefaultDescriptionFromParameters
		(
			UINT width, UINT height, UINT bindFlags
		);

		bool IsValidBindFlags(UINT bindFlags);

		std::shared_ptr<DeviceResources> deviceResources;

		winrt::com_ptr<ID3D11Texture2D> texture;
		DXGI_FORMAT format;
		UINT bindFlags;
		winrt::com_ptr<ID3D11ShaderResourceView> shaderResourceView;
		winrt::com_ptr<ID3D11RenderTargetView> renderTargetView;
		winrt::com_ptr<ID3D11DepthStencilView> depthStencilView;
	};
}