#include "pch.h"

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
	HWND windowHandle = CreateWindowEx(
		0, className, L"Aerial", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200, 200, 1600, 900,
		nullptr, nullptr, hInstance, nullptr
	);

	//gfx = new Graphics(windowHandle);
	ShowWindow(windowHandle, SW_SHOW);

	// message pump
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		Update(msg);
	}

	return msg.wParam;
}