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
	deviceResources(deviceResources), camera(nullptr), lightConstantBuffer(nullptr), 
	lights(DirectionalLight{ {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, -1.0f, 1.0f}, 0.0f} ),
	mainShadowMap
	(
		deviceResources, shadowMapDimensions.x, shadowMapDimensions.y
	)
{
	LoadAllShaders();
	camera = make_unique<Camera>(deviceResources->AspectRatio(), 0.1f, 1000.0f, deviceResources);
	lightConstantBuffer = make_unique<PixelConstantBuffer<LightBuffer>>(deviceResources, lights, 7);
	OutputDebugString(L"Core Renderer ready \n");
}

// Load unlit shader for debug purposes
// TODO: refactor into (Shader Cache + Material) system in support of custom shader
void CoreRenderPipeline::LoadAllShaders()
{
	PixelShader forwardLitPixelShader(deviceResources, L"ForwardLit.cso");
	VertexShader litVertexShader(deviceResources, L"LitVertexShader.cso", VertexPNTTLayout, (UINT)size(VertexPNTTLayout));
	forwardLitPixelShader.Bind();
	litVertexShader.Bind();
}

void CoreRenderPipeline::StartGUIFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

// TODO: refactor actual windows configuration into its own class
void CoreRenderPipeline::DrawGUI()
{
	ImGui::Begin("Camera Control");
	XMFLOAT3 cameraPosition;
	XMVECTOR pos = camera->GetTransform()->WorldPosition();
	XMStoreFloat3(&cameraPosition, pos);
	ImGui::Text
	(
		"Camera world space position \n (%.2f, %.2f, %.2f) \n",
		cameraPosition.x, cameraPosition.y, cameraPosition.z
	);
	ImGui::Text
	(
		"Camera pitch yaw \n (%.2f, %.2f) \n",
		camera->Pitch(), camera->Yaw()
	);
	if (ImGui::Button("Reset Camera"))
	{
		ResetCamera();
	}
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void CoreRenderPipeline::ResetCamera()
{
	camera->GetTransform()->UpdateTransform(XMMatrixIdentity());
	camera->Pitch(0.0f);
	camera->Yaw(0.0f);
}

void CoreRenderPipeline::ShadowPass()
{
	deviceResources->Context()->ClearDepthStencilView
	(
		mainShadowMap.UseAsDepthStencil().get(), D3D11_CLEAR_DEPTH, 1.0f, 0
	);
	deviceResources->SetTargets(0, nullptr, mainShadowMap.UseAsDepthStencil().get());

	// TODO: Draw from the perspective of light

	for (GameObject*& gameObject : Scene::Instance()->ObjectsInScene)
	{
		gameObject->Render();
	}
}

void CoreRenderPipeline::SimpleForwardPass()
{
	deviceResources->SetTargetsToBackBuffer();	// Always set target to current back buffer before drawing
	deviceResources->ClearFrame();		// Clear the view before we start drawing

	camera->BindCamera2Pipeline();		// Render from the perspective of the camera

	// Update & bind all the lights
	lightConstantBuffer->Update(lights);
	lightConstantBuffer->Bind();

	// For each object we render it in a single pass
	for (GameObject*& gameObject : Scene::Instance()->ObjectsInScene)
	{
		gameObject->Render();
	}
}

void CoreRenderPipeline::Render()
{
	StartGUIFrame();

	camera->Update();

	ShadowPass();
	SimpleForwardPass();

	// Draw GUI on top of the game
	DrawGUI();
	deviceResources->Present();
}
