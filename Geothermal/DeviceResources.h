#pragma once

namespace Geothermal::Graphics
{
	/// <summary>
	/// Core wrapper for Direct3D graphic API device resources
	/// Provide access to D3D11 device, device context and DXGI swap chain
	/// </summary>
	class DeviceResources
	{
	public:
		DeviceResources();
		void SetWindow(HWND windowHandle, UINT width, UINT height);
		void Present();

		ID3D11Device* D3DDevice() const { return d3dDevice.get(); }
		ID3D11DeviceContext3* D3DDeviceContext() const { return d3dContext.get(); }
		IDXGISwapChain1* SwapChain() const { return swapChain.get(); }
		float AspectRatio() const { return (float)outputSize.x / (float)outputSize.y; }

		void ClearView();
		void SetTargets();
	private:
		void CreateDeviceResources();
		void CreateWindowSizeDependentResources(HWND windowHandle);
		void ClearPreviousSizeDependentResources();

		// Direct3D basic components
		winrt::com_ptr<ID3D11Device> d3dDevice;
		winrt::com_ptr<ID3D11DeviceContext3> d3dContext;
		winrt::com_ptr<IDXGISwapChain1>       swapChain;

		winrt::com_ptr<ID3D11RenderTargetView>  d3dRenderTargetView;
		D3D_FEATURE_LEVEL d3dFeatureLevel;
		D3D11_VIEWPORT screenViewPort;

		const FLOAT ClearColor[4] = { 0.0f,0.0f,0.0f,1.0f };

		// Window-related resources
		DirectX::XMUINT2 outputSize;
	};
}