#include "pch.h"
#include "DeviceResources.h"

std::shared_ptr<Geothermal::Graphics::DeviceResources> deviceResources;

LRESULT CALLBACK WindowProcedure(HWND windowsHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(1);
		break;
	}
	return DefWindowProc(windowsHandle, msg, wParam, lParam);
}

void Update(MSG msg)
{
	TranslateMessage(&msg);
	DispatchMessage(&msg);

	deviceResources->SetTargets();
	deviceResources->ClearView();
	deviceResources->Present();
}

int CALLBACK WinMain
(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	const auto className = L"cuo";
	// Register window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WindowProcedure;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = className;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);

	// Create instance of the window
	const UINT width = 1600;
	const UINT height = 900;

	HWND windowHandle = CreateWindowEx(
		0, className, L"Geothermal", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200, 200, width, height,
		nullptr, nullptr, hInstance, nullptr
	);

	ShowWindow(windowHandle, SW_SHOW);

	deviceResources =
		std::make_shared<Geothermal::Graphics::DeviceResources>(windowHandle);
	//deviceResources->SetWindow(windowHandle, width, height);
	deviceResources->SetTargets();
	deviceResources->ClearView();
	deviceResources->Present();

	// message pump
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		Update(msg);
	}

	return msg.wParam;
}