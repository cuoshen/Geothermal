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
	shadowViewPort = CD3D11_VIEWPORT(
		0.0f,
		0.0f,
		shadowMapDimensions.x,
		shadowMapDimensions.y
	);
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
	deviceResources->Context()->RSSetViewports(1, &shadowViewPort);
	deviceResources->SetTargets(0, nullptr, mainShadowMap.UseAsDepthStencil().get());

	// Render from the perspective of the main light
	mainLightViewParameters.CameraWorldPosition = mainLightShadowCastingOrigin;
	XMVECTOR position = XMLoadFloat3(&mainLightShadowCastingOrigin);
	XMVECTOR direction = XMLoadFloat3(&lights.MainLight.Direction);
	XMFLOAT3 upFloat3 = XMFLOAT3{ 0.0f, 1.0f, 0.0f };
	XMVECTOR up = XMLoadFloat3(&upFloat3);
	mainLightViewParameters.World2ClipTransform = 
		XMMatrixLookToLH(position, direction, up) * XMMatrixOrthographicLH(30.0f, 30.0f, 0.0f, 1000.0f);

	VertexConstantBuffer<CameraParameters> parametersBufferVS(deviceResources, mainLightViewParameters, 1u);
	PixelConstantBuffer<CameraParameters> parametersBufferPS(deviceResources, mainLightViewParameters, 1u);
	parametersBufferVS.Bind();
	parametersBufferPS.Bind();

	for (GameObject*& gameObject : Scene::Instance()->ObjectsInScene)
	{
		gameObject->Render();
	}
}

void CoreRenderPipeline::SimpleForwardPass()
{
	deviceResources->SetTargetsToBackBuffer();	// Always set target to current back buffer before drawing
	deviceResources->ClearFrame();		// Clear the view before we start drawing

	camera->BindCamera2Pipeline();		// Render from the perspective of the main camera

	// Update & bind all the lights
	lightConstantBuffer->Update(lights);
	lightConstantBuffer->Bind();

	// For each object we render it in a single pass
	// TODO: sort objects
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
