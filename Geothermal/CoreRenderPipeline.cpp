#include "pch.h"
#include <d3dcompiler.h>
#include "CoreRenderPipeline.h"
#include "Vertices.h"
#include "Shaders.h"
#include "GameMain.h"
#include "Scene.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

using namespace Geothermal::Graphics;
using namespace Bindables;
using namespace Structures;
using namespace std;
using namespace DirectX;

CoreRenderPipeline::CoreRenderPipeline(std::shared_ptr<DeviceResources> const& deviceResources):
	deviceResources(deviceResources), camera(nullptr)
{
	LoadAllShaders();
	camera = make_unique<Camera>(deviceResources->AspectRatio(), 0.1f, 100.0f, deviceResources);
	OutputDebugString(L"Core Renderer ready \n");
}

// Load unlit shader for debug purposes
// TODO: refactor into (Shader Cache + Material) system in support of custom shader
void CoreRenderPipeline::LoadAllShaders()
{
	PixelShader unlitPixelShader(deviceResources, L"TexturedForwardLit.cso");
	VertexShader defaultVertexShader(deviceResources, L"LitVertexShader.cso", VertexPNTTLayout, (UINT)size(VertexPNTTLayout));
	unlitPixelShader.Bind();
	defaultVertexShader.Bind();
}

void CoreRenderPipeline::StartGUIFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void CoreRenderPipeline::DrawGUI()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void CoreRenderPipeline::Render()
{
	deviceResources->SetTargets();	// Always set target to current back buffer before drawing
	deviceResources->ClearView();		// Clear the view before we start drawing

	StartGUIFrame();

	camera->Update();
	camera->BindCamera2Pipeline();

	// Simple forward rendering
	// For each object we render it in a single pass
	for (GameObject*& gameObject : Scene::Instance()->ObjectsInScene)
	{
		gameObject->Render();
	}

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::End();
	// Draw GUI on top of the game
	DrawGUI();
	deviceResources->Present();
}
