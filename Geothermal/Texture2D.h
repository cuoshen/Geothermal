#pragma once
#include "DeviceResources.h"
#include "pch.h"

namespace Geothermal::Graphics
{
	// TODO: find a subnamespace to put this class

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
	public:
		Texture2D(std::shared_ptr<DeviceResources> deviceResources, winrt::hstring const& filename, TEXTURE_FILE_TYPE fileType);
		Texture2D(std::shared_ptr<DeviceResources> deviceResources, std::vector<char> data, DXGI_FORMAT format, UINT width, UINT height, UINT bitsPerPixel);

		winrt::com_ptr<ID3D11ShaderResourceView> UseAsShaderResource();
		winrt::com_ptr<ID3D11RenderTargetView> UseAsRenderTarget();
	private:
		void CreateTextureFromMemory(std::vector<char> data, UINT width, UINT height, UINT bitsPerPixel);
		void CreateShaderResourceView();
		void CreateRenderTargetView();

		std::shared_ptr<DeviceResources> deviceResources;

		winrt::com_ptr<ID3D11Texture2D> texture;
		DXGI_FORMAT format;
		winrt::com_ptr<ID3D11ShaderResourceView> shaderResourceView;
		winrt::com_ptr<ID3D11RenderTargetView> renderTargetView;
	};
}