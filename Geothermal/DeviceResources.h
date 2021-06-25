#pragma once

namespace Aeriel::Graphics
{
	/// <summary>
	/// Core wrapper for Direct3D graphic API device resources
	/// Provide access to D3D11 device, device context and DXGI swap chain
	/// </summary>
	class DeviceResources
	{
	public:
		DeviceResources();
		void SetWindow();
		void Present();

		ID3D11Device* GetD3DDevice() const { return d3dDevice.get(); }
		ID3D11DeviceContext3* GetD3DDeviceContext() const { return d3dContext.get(); }
		IDXGISwapChain1* GetSwapChain() const { return swapChain.get(); }
		float GetAspectRatio() const { return (float)outputSize.Width / (float)outputSize.Height; }

		void ClearView();
		void SetTargets();
	private:
		void CreateDeviceResources();
		void CreateWindowSizeDependentResources();
		void ClearPreviousSizeDependentResources();

		// Direct3D basic components
		winrt::com_ptr<ID3D11Device> d3dDevice;
		winrt::com_ptr<ID3D11DeviceContext3> d3dContext;
		winrt::com_ptr<IDXGISwapChain1>       swapChain;

		winrt::com_ptr<ID3D11RenderTargetView>  d3dRenderTargetView;
		D3D_FEATURE_LEVEL d3dFeatureLevel;
		D3D11_VIEWPORT screenViewPort;

		const FLOAT ClearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

		// Window-related resources
		winrt::Windows::Foundation::Size logicalSize;
		winrt::Windows::Foundation::Size outputSize;
		float dpi;
	};
}