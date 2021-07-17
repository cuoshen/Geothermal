#include "pch.h"
#include "DeviceResources.h"
//#include "Ultilities.h"
#include <math.h>
#include <unknwn.h>

using namespace Geothermal::Graphics;
using namespace D2D1;
using namespace DirectX;
using namespace winrt;

DeviceResources::DeviceResources():
    d3dFeatureLevel(D3D_FEATURE_LEVEL_11_0)
{
    CreateDeviceResources();
    OutputDebugString(L"DeviceResources created \n");
}

// Create Direct3D device and context
void DeviceResources::CreateDeviceResources()
{
    // This array defines the set of DirectX hardware feature levels this app will support.
    // Note the ordering should be preserved.
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    // Create the Direct3D 11 API device object and a corresponding context.
    com_ptr<ID3D11Device> device;
    com_ptr<ID3D11DeviceContext> context;

    UINT creationFlags = 0;

//#if defined(_DEBUG)
//    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif

    HRESULT hr = D3D11CreateDevice(
        nullptr,                                                    // Specify nullptr to use the default adapter.
        D3D_DRIVER_TYPE_HARDWARE,  // Create a device using the hardware graphics driver.
        0,                                                             // Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
        creationFlags,                                      // Set debug flags.
        featureLevels,                                      // List of feature levels this app can support.
        ARRAYSIZE(featureLevels),              // Size of the list above.
        D3D11_SDK_VERSION,                    // Always set this to D3D11_SDK_VERSION for Windows Runtime apps.
        device.put(),                                        // Returns the Direct3D device created.
        &d3dFeatureLevel,                            // Returns feature level of device created.
        context.put()                                       // Returns the device immediate context.
    );
    winrt::check_hresult(hr);

    d3dDevice = device;
    d3dContext = context.as<ID3D11DeviceContext3>();
}

// Parse out window dimensions and create swap chain accordingly
void DeviceResources::SetWindow(HWND windowHandle, UINT width, UINT height)
{
    outputSize = XMUINT2{ width, height };
    CreateWindowSizeDependentResources(windowHandle);
}

void DeviceResources::CreateWindowSizeDependentResources(HWND windowHandle)
{
    ClearPreviousSizeDependentResources();

    outputSize.x = max(1, outputSize.x);
    outputSize.y = max(1, outputSize.y);

    if (swapChain != nullptr)
    {
        // The swap chain already exists, resize it
        HRESULT hr = swapChain->ResizeBuffers(
            2,  // Double buffered
            outputSize.x,
            outputSize.y,
            DXGI_FORMAT_B8G8R8A8_UNORM,
            0   // No flags
        );

        check_hresult(hr);
    }
    else
    {
        // Create new swap chain
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };

        swapChainDesc.Width = outputSize.x;
        swapChainDesc.Height = outputSize.y;
        swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // This is the most common swap chain format.
        swapChainDesc.SampleDesc.Count = 1; // Don't use multi-sampling.
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 2; // Double buffer
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All Windows Store apps must use this SwapEffect.
        swapChainDesc.Flags = 0;
        swapChainDesc.Scaling = DXGI_SCALING_NONE;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

        winrt::com_ptr<IDXGIDevice3> dxgiDevice;
        dxgiDevice = d3dDevice.as<IDXGIDevice3>();  // Use the device we created
        winrt::com_ptr<IDXGIAdapter> dxgiAdapter;
        winrt::check_hresult(
            dxgiDevice->GetAdapter(dxgiAdapter.put())
        );

        winrt::com_ptr<IDXGIFactory3> dxgiFactory;
        winrt::check_hresult(
            dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory))
        );

        winrt::check_hresult(
            dxgiFactory->CreateSwapChainForHwnd(
                d3dDevice.get(),
                windowHandle,
                &swapChainDesc,
                nullptr,
                nullptr,
                swapChain.put()
            )
        );

        winrt::check_hresult(
            dxgiDevice->SetMaximumFrameLatency(1)
        );

        OutputDebugString(L"Swap chain created successfully \n");
    }

    // After processing swap chain, we create a render target view of the swap chain back buffer
    winrt::com_ptr<ID3D11Texture2D> backBuffer = winrt::capture<ID3D11Texture2D>(swapChain, &IDXGISwapChain1::GetBuffer, 0);

    winrt::check_hresult(
        d3dDevice->CreateRenderTargetView(
            backBuffer.get(),
            nullptr,
            backBufferTargetView.put()
        )
    );

    // Create a depth stencil view for use with 3D rendering if needed.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        outputSize.x,
        outputSize.y,
        1,
        1,
        D3D11_BIND_DEPTH_STENCIL
    );

    winrt::com_ptr<ID3D11Texture2D> depthStencil(nullptr);
    winrt::check_hresult(
        d3dDevice->CreateTexture2D(
            &depthStencilDesc,
            nullptr,
            depthStencil.put()
        )
    );

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    winrt::check_hresult(
        d3dDevice->CreateDepthStencilView(
            depthStencil.get(),
            &depthStencilViewDesc,
            depthStencilView.put()
        )
    );

    // Bind viewport
    screenViewPort = CD3D11_VIEWPORT(
        0.0f,
        0.0f,
        outputSize.x,
        outputSize.y
    );
    d3dContext->RSSetViewports(1, &screenViewPort);

    SetTargetsToBackBuffer();
}

void DeviceResources::ClearPreviousSizeDependentResources()
{
    ID3D11RenderTargetView* nullViews[] = { nullptr };
    d3dContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
    d3dContext->Flush();
}

void DeviceResources::Present()
{
    winrt::check_hresult(
        swapChain->Present(1, 0)
    );
    d3dContext->DiscardView(backBufferTargetView.get());
    d3dContext->DiscardView(depthStencilView.get());
}

void DeviceResources::ClearFrame()
{
    d3dContext->ClearRenderTargetView(backBufferTargetView.get(), ClearColor);
    d3dContext->ClearDepthStencilView(depthStencilView.get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DeviceResources::SetTargetsToBackBuffer()
{
    d3dContext->RSSetViewports(1, &screenViewPort);
    ID3D11RenderTargetView* target = backBufferTargetView.get();
    SetTargets(1, &target, depthStencilView.get());
}

void DeviceResources::SetTargets
(
    UINT numberOfViews, 
    ID3D11RenderTargetView** targets, 
    ID3D11DepthStencilView* depthStencilView
)
{
    // Simply wraps OMSetRenderTargets
    d3dContext->OMSetRenderTargets(numberOfViews, targets, depthStencilView);
}
