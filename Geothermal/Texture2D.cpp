#include "pch.h"
#include "Texture2D.h"

using namespace std;
using namespace winrt;
using namespace Geothermal::Graphics;


Texture2D::Texture2D(shared_ptr<DeviceResources> deviceResources, hstring const& filename, TEXTURE_FILE_TYPE fileType):
	texture(nullptr), shaderResourceView(nullptr), renderTargetView(nullptr), format(DXGI_FORMAT_R8G8B8A8_SNORM)
{
	// Read file into buffer, TODO: maybe refactor into FileIO or a separate class

	// Create factory
	com_ptr<IWICImagingFactory> factory;
	check_hresult(
		CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			__uuidof(IWICImagingFactory),
			(LPVOID*)factory.get()			// We pass in * instead of ** here
		)
	);
	
	com_ptr<IWICBitmapDecoder> decoder;
	check_hresult(
		factory->CreateDecoderFromFilename(filename.c_str(), 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, decoder.put())
	);
	com_ptr<IWICBitmapFrameDecode> frame;
	check_hresult(
		decoder->GetFrame(0, frame.put())
	);

	// Create texture
	D3D11_TEXTURE2D_DESC description = {0};

}

Texture2D::Texture2D
(
	std::shared_ptr<DeviceResources> deviceResources, std::vector<char> data, 
	DXGI_FORMAT format, UINT width, UINT height, UINT bitsPerPixel
):
	texture(nullptr), shaderResourceView(nullptr), renderTargetView(nullptr), format(format)
{
	CreateTextureFromMemory(data, width, height, bitsPerPixel);
}

void Texture2D::CreateTextureFromMemory(vector<char> data, UINT width, UINT height, UINT bitsPerPixel)
{
	D3D11_TEXTURE2D_DESC description = { 0 };
	description.Width = width;
	description.Height = height;
	description.MipLevels = 1;
	description.ArraySize = 1;
	description.Format = format;
	description.SampleDesc.Count = 1;
	description.SampleDesc.Quality = 0;
	description.Usage = D3D11_USAGE_DEFAULT;
	description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	description.CPUAccessFlags = 0;
	description.MiscFlags = 0;

	// Take quotient
	size_t rowPitch = (width * bitsPerPixel + 7) / 8;
	size_t imageSize = rowPitch * height;

	D3D11_SUBRESOURCE_DATA initialData;
	initialData.pSysMem = data.data();
	initialData.SysMemPitch = static_cast<UINT>(rowPitch);
	initialData.SysMemSlicePitch = static_cast<UINT>(imageSize);

	check_hresult(
		deviceResources->D3DDevice()->CreateTexture2D(&description, &initialData, texture.put())
	);
}

com_ptr<ID3D11ShaderResourceView> Texture2D::UseAsShaderResource()
{
	if (shaderResourceView == nullptr)
	{
		CreateShaderResourceView();
	}
	return shaderResourceView;
}

com_ptr<ID3D11RenderTargetView> Texture2D::UseAsRenderTarget()
{
	if (renderTargetView == nullptr)
	{
		CreateRenderTargetView();
	}
	return renderTargetView;
}

void Texture2D::CreateShaderResourceView()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDescription = {};
	SRVDescription.Format = format;
	SRVDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDescription.Texture2D.MipLevels = 1;

	check_hresult(
		deviceResources->D3DDevice()->CreateShaderResourceView(texture.get(), &SRVDescription, shaderResourceView.put())
	);
}

void Texture2D::CreateRenderTargetView()
{
	D3D11_RENDER_TARGET_VIEW_DESC RTVDescription = {};
	RTVDescription.Format = format;
	RTVDescription.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	
	check_hresult(
		deviceResources->D3DDevice()->CreateRenderTargetView(texture.get(), &RTVDescription, renderTargetView.put())
	);
}
