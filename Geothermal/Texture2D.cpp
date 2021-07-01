#include "pch.h"
#include "Texture2D.h"

using namespace std;
using namespace winrt;
using namespace Geothermal::Graphics;


Texture2D::Texture2D(shared_ptr<DeviceResources> deviceResources, hstring const& filename, TEXTURE_FILE_TYPE fileType)
{
	// Read file into buffer

	// Create texture
	D3D11_TEXTURE2D_DESC description = {0};

}

com_ptr<ID3D11ShaderResourceView> Texture2D::UseAsShaderResource()
{
	return com_ptr<ID3D11ShaderResourceView>();
}

com_ptr<ID3D11RenderTargetView> Texture2D::UseAsRenderTarget()
{
	return com_ptr<ID3D11RenderTargetView>();
}
