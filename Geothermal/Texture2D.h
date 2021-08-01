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
			winrt::hstring const& filename, TEXTURE_FILE_TYPE fileType);

		/// <summary>
		/// For non-DDS image file loaded into the memory, we need to pass in metadata
		/// </summary>
		Texture2D(std::shared_ptr<DeviceResources> const& deviceResources, std::vector<char> data, 
			DXGI_FORMAT format, uint width, uint height, uint bitsPerPixel, uint bindFlags);

		/// <summary>
		/// Build one from scratch in given dimensions
		/// </summary>
		Texture2D(std::shared_ptr<DeviceResources> const& deviceResources,
			DXGI_FORMAT format, uint width, uint height, uint bindFlags);

		winrt::com_ptr<ID3D11ShaderResourceView> UseAsShaderResource();
		winrt::com_ptr<ID3D11RenderTargetView> UseAsRenderTarget();
		winrt::com_ptr<ID3D11DepthStencilView> UseAsDepthStencil();

	private:
		void CreateTextureFromMemory
		(
			std::vector<char> data, uint width, uint height, uint bitsPerPixel, uint bingFlags
		);
		void CreateShaderResourceView();
		void CreateRenderTargetView();
		void CreateDepthStencilView();

		D3D11_TEXTURE2D_DESC DefaultDescriptionFromParameters
		(
			uint width, uint height, uint bindFlags
		);

		bool IsValidBindFlags(uint bindFlags);

		std::shared_ptr<DeviceResources> deviceResources;

		winrt::com_ptr<ID3D11Texture2D> texture;
		DXGI_FORMAT format;
		uint bindFlags;
		winrt::com_ptr<ID3D11ShaderResourceView> shaderResourceView;
		winrt::com_ptr<ID3D11RenderTargetView> renderTargetView;
		winrt::com_ptr<ID3D11DepthStencilView> depthStencilView;
	};
}
