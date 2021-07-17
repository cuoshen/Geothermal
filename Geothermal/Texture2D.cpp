#include "pch.h"
#include "Texture2D.h"

using namespace std;
using namespace winrt;
using namespace DirectX;
using namespace Geothermal::Graphics;

Texture2D::Texture2D
(
	shared_ptr<DeviceResources> const& deviceResources, 
	hstring const& filename, TEXTURE_FILE_TYPE fileType
):
	deviceResources(deviceResources),
	texture(nullptr), 
	shaderResourceView(nullptr), renderTargetView(nullptr), depthStencilView(nullptr),
	format(DXGI_FORMAT_R8G8B8A8_SNORM),
	// If we create a texture from file, usually, we don't want to override the content
	// thus we use it only as a shader resource
	bindFlags(D3D11_BIND_SHADER_RESOURCE)
{
	D3D11_TEXTURE2D_DESC description = { 0 };

	if (fileType == DDS)
	{
		// Use DDSTextureLoader

		com_ptr<ID3D11Resource> resource;
		check_hresult
		(
			CreateDDSTextureFromFile
			(
				deviceResources->Device(),
				filename.c_str(),
				resource.put(),
				nullptr
			)
		);

		D3D11_RESOURCE_DIMENSION resourceType = 
			D3D11_RESOURCE_DIMENSION_UNKNOWN;
		resource->GetType(&resourceType);
		assert(resourceType == D3D11_RESOURCE_DIMENSION_TEXTURE2D);

		texture = resource.as<ID3D11Texture2D>();

		return;
	}

	// Use WIC to load the image

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
		factory->CreateDecoderFromFilename
		(
			filename.c_str(), 
			0, 
			GENERIC_READ, 
			WICDecodeMetadataCacheOnDemand, 
			decoder.put()
		)
	);
	com_ptr<IWICBitmapFrameDecode> frame;
	check_hresult(
		decoder->GetFrame(0, frame.put())
	);

	// TODO: Create texture
}

Texture2D::Texture2D
(
	shared_ptr<DeviceResources> const& deviceResources, vector<char> data,
	DXGI_FORMAT format, UINT width, UINT height, UINT bitsPerPixel, UINT bindFlags
):
	deviceResources(deviceResources),
	texture(nullptr), 
	shaderResourceView(nullptr), renderTargetView(nullptr), depthStencilView(nullptr),
	format(format),
	bindFlags(bindFlags)
{
	assert(IsValidBindFlags(bindFlags));

	CreateTextureFromMemory(data, width, height, bitsPerPixel, bindFlags);
}

void Texture2D::CreateTextureFromMemory(vector<char> data, UINT width, UINT height, UINT bitsPerPixel, UINT bindFlags)
{
	D3D11_TEXTURE2D_DESC description = DefaultDescriptionFromParameters(width, height, bindFlags);

	// Take quotient
	size_t rowPitch = (width * bitsPerPixel + 7) / 8;
	size_t imageSize = rowPitch * height;

	D3D11_SUBRESOURCE_DATA initialData;
	initialData.pSysMem = data.data();
	initialData.SysMemPitch = static_cast<UINT>(rowPitch);
	initialData.SysMemSlicePitch = static_cast<UINT>(imageSize);

	check_hresult(
		deviceResources->Device()->CreateTexture2D(&description, &initialData, texture.put())
	);
}

Texture2D::Texture2D
(
	shared_ptr<DeviceResources> const& deviceResources, DXGI_FORMAT format, UINT width, UINT height, UINT bindFlags
) :
	deviceResources(deviceResources),
	texture(nullptr), 
	shaderResourceView(nullptr), renderTargetView(nullptr), depthStencilView(nullptr), 
	format(format),
	bindFlags(bindFlags)
{
	assert(IsValidBindFlags(bindFlags));

	D3D11_TEXTURE2D_DESC description = DefaultDescriptionFromParameters(width, height, bindFlags);

	check_hresult(
		deviceResources->Device()->CreateTexture2D(&description, nullptr, texture.put())
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

com_ptr<ID3D11DepthStencilView> Texture2D::UseAsDepthStencil()
{
	if (depthStencilView == nullptr)
	{
		CreateDepthStencilView();
	}
	return depthStencilView;
}

void Texture2D::CreateShaderResourceView()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDescription = {};
	SRVDescription.Format = format;
	SRVDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDescription.Texture2D.MipLevels = 1;

	check_hresult(
		deviceResources->Device()->CreateShaderResourceView(texture.get(), nullptr, shaderResourceView.put())
	);
}

void Texture2D::CreateRenderTargetView()
{
	D3D11_RENDER_TARGET_VIEW_DESC RTVDescription = {};
	RTVDescription.Format = format;
	RTVDescription.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	
	check_hresult(
		deviceResources->Device()->CreateRenderTargetView(texture.get(), &RTVDescription, renderTargetView.put())
	);
}

void Texture2D::CreateDepthStencilView()
{
	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;

	winrt::check_hresult(
		deviceResources->Device()->CreateDepthStencilView(
			texture.get(),
			&depthStencilViewDesc,
			depthStencilView.put()
		)
	);
}

D3D11_TEXTURE2D_DESC Texture2D::DefaultDescriptionFromParameters(UINT width, UINT height, UINT bindFlags)
{
	D3D11_TEXTURE2D_DESC description = { 0 };
	description.Width = width;
	description.Height = height;
	description.MipLevels = 0;
	description.ArraySize = 1;
	description.Format = format;
	description.SampleDesc.Count = 1;
	description.SampleDesc.Quality = 0;
	description.Usage = D3D11_USAGE_DEFAULT;
	description.BindFlags = bindFlags;
	description.CPUAccessFlags = 0;
	description.MiscFlags = 0;

	return description;
}

bool Texture2D::IsValidBindFlags(UINT bindFlags)
{
	bool isShaderResource = bindFlags & D3D11_BIND_SHADER_RESOURCE;
	bool isRenderTarget = bindFlags & D3D11_BIND_RENDER_TARGET;
	bool isDepthStencil = bindFlags & D3D11_BIND_DEPTH_STENCIL;

	// We cannot use the same texture as render target view AND depth stencil view, thus the xor
	// We want to use the texture for SOMETHING of value, thus we should have at least 1 valid bindflag
	return (isRenderTarget != isDepthStencil) && (isShaderResource || isRenderTarget || isDepthStencil);
}
