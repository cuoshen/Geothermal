#include "pch.h"
#include "DeviceResources.h"
#include "GameMain.h"

LRESULT CALLBACK WindowProcedure(HWND windowsHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		PostQuitMessage(1);
		break;
	}
	return DefWindowProc(windowsHandle, message, wParam, lParam);
}

UINT Update(MSG msg)
{
	if (msg.message == WM_QUIT)
	{
		return 1;
	}

	TranslateMessage(&msg);
	DispatchMessage(&msg);

	OutputDebugString(L"update called \n");

	GameMain::Instance()->GetInput()->RegisterInput(&msg);

	return 0;
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

	std::shared_ptr<Geothermal::Graphics::DeviceResources> deviceResources =
		std::make_shared<Geothermal::Graphics::DeviceResources>();
	deviceResources->SetWindow(windowHandle, width, height);
	
	GameMain::Initialize(deviceResources);

	// message pump
	MSG msg = {0};
	while (true)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) > 0)
		{
			UINT result = Update(msg);
			if (result != 0)
			{
				return msg.wParam;
			}
		}
		GameMain::Instance()->Update();
		GameMain::Instance()->LateUpdate();
	}

	return msg.wParam;
}