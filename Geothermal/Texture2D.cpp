#include "pch.h"
#include "Texture2D.h"

using namespace std;
using namespace winrt;
using namespace Geothermal::Graphics;


Texture2D::Texture2D(shared_ptr<DeviceResources> deviceResources, hstring const& filename, TEXTURE_FILE_TYPE fileType):
	texture(nullptr), shaderResourceView(nullptr), renderTargetView(nullptr), format(DXGI_FORMAT_R8G8B8A8_SNORM)
{
	// Read file into buffer

	// Create texture
	D3D11_TEXTURE2D_DESC description = {0};

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
