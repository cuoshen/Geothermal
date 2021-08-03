#pragma once

#include "CommonStates.h"

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

		// Accessors

		ID3D11Device* Device() const { return d3dDevice.get(); }
		ID3D11DeviceContext3* Context() const { return d3dContext.get(); }
		IDXGISwapChain1* SwapChain() const { return swapChain.get(); }
		float AspectRatio() const { return (float)outputSize.x / (float)outputSize.y; }
		ID3D11RenderTargetView* BackBufferTargetView() const { return backBufferTargetView.get(); }
		ID3D11DepthStencilView* DepthStencilView() const { return depthStencilView.get(); }
		const D3D11_VIEWPORT& ScreenViewport() const { return screenViewPort; }
		const DirectX::XMUINT2& OutputSize() const { return outputSize; }

		/// <summary>
		/// Clear back buffer to constant ClearColor.
		/// Clear depth stencil buffer to far Z = 1.
		/// </summary>
		void ClearFrame();

		/// <summary>
		/// Reset BlendState, DepthStencilState and RasterizerState
		/// </summary>
		void ResetDefaultPipelineStates();

		/// <summary>
		/// Set the output of OM stage to targets
		/// </summary>
		void SetTargets
		(
			UINT numberOfViews,
			ID3D11RenderTargetView** targets,
			ID3D11DepthStencilView* depthStencilView
		);

		const float ClearColor[4] = { 0.0f,0.0f,0.0f,1.0f };
	private:
		void CreateDeviceResources();
		void CreateWindowSizeDependentResources(HWND windowHandle);
		void ClearPreviousSizeDependentResources();

		// Direct3D basic components
		winrt::com_ptr<ID3D11Device> d3dDevice;
		winrt::com_ptr<ID3D11DeviceContext3> d3dContext;
		winrt::com_ptr<IDXGISwapChain1>       swapChain;

		winrt::com_ptr<ID3D11RenderTargetView> backBufferTargetView;
		winrt::com_ptr<ID3D11DepthStencilView> depthStencilView;
		D3D_FEATURE_LEVEL d3dFeatureLevel;
		D3D11_VIEWPORT screenViewPort;

		/// <summary>
		/// x stands for width, y stands for height
		/// </summary>
		DirectX::XMUINT2 outputSize;

		std::unique_ptr<DirectX::CommonStates> states;
	};
}