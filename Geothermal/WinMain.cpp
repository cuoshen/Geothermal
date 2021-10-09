#include "pch.h"
#include "DeviceResources.h"
#include "GameMain.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

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

void RegisterWindowsClass(LPCWSTR className, HINSTANCE hInstance)
{
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
}

void SetupGUI
(
	HWND windowHandle,
	std::shared_ptr<Geothermal::Graphics::DeviceResources> const& deviceResources
)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(windowHandle);
	ImGui_ImplDX11_Init(deviceResources->Device(), deviceResources->Context());
}

void CleanupGUI()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
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
	RegisterWindowsClass(className, hInstance);

	const UINT width = 1600;
	const UINT height = 900;

	// Create instance of the window
	HWND windowHandle = CreateWindowEx(
		0, className, L"Geothermal", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200, 200, width, height,
		nullptr, nullptr, hInstance, nullptr
	);

	ShowWindow(windowHandle, SW_SHOW);

	std::shared_ptr<Geothermal::Graphics::DeviceResources> deviceResources =
		std::make_shared<Geothermal::Graphics::DeviceResources>();
	deviceResources->SetWindow(windowHandle, width, height);

	Geothermal::GameMain::Initialize(deviceResources);
	SetupGUI(windowHandle, deviceResources);
	// Run the game
	WPARAM exitResult = Geothermal::GameMain::Instance()->Run();

	CleanupGUI();

	return exitResult;
}
