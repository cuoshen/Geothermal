#include "pch.h"
#include <d3dcompiler.h>
#include "CoreRenderPipeline.h"
#include "GraphicResources.h"
#include "ShaderCache.h"
#include "Material.h"
#include "GameMain.h"
#include "Scene.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

using namespace Geothermal::Graphics;
using namespace Bindables;
using namespace Structures;
using namespace Passes;
using namespace std;
using namespace DirectX;

CoreRenderPipeline::CoreRenderPipeline(std::shared_ptr<DeviceResources> const& deviceResources) :
	deviceResources(deviceResources), camera(nullptr), lightsConstantBuffer(nullptr),
	lights(DirectionalLight{ {1.0f, 1.0f, 1.0f, 1.0f}, {0.2f, -1.0f, 1.0f}, 0.0f }), bloomThreshold(1.0f),
	shadowCaster(deviceResources, 30.0f, 30.0f, 0.0f, 1000.0f), bloomSize(5.0f), bloomBrightness(3.0f),
	ShadowCasterParametersBuffer(deviceResources, 5u),  exposure(0.0f),
	mainShadowMap(nullptr), useBloom(false),
	simpleForwardPass(nullptr), postProcessingPass(nullptr)
{
	ShaderCache::Initialize(deviceResources);

	camera = make_unique<Camera>(deviceResources->AspectRatio(), 0.1f, 1000.0f, deviceResources);
	lightsConstantBuffer = make_unique<PixelConstantBuffer<LightBuffer>>(deviceResources, lights, 7);
	shadowViewPort = CD3D11_VIEWPORT(
		0.0f,
		0.0f,
		shadowMapDimensions.x,
		shadowMapDimensions.y
	);
	mainShadowMap =
		make_unique<ShadowMap>(deviceResources, shadowMapDimensions.x, shadowMapDimensions.y);

	for (int i = 0; i < 2; i++)
	{
		hdrSceneRenderTarget[i] = make_unique<Texture2D>
		(
		deviceResources, DXGI_FORMAT_R32G32B32A32_FLOAT,
		deviceResources->OutputSize().x, deviceResources->OutputSize().y,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, 0u
		);
		bloomTextures[i] = make_unique<Texture2D>
		(
			deviceResources, DXGI_FORMAT_R32G32B32A32_FLOAT,
			deviceResources->OutputSize().x, deviceResources->OutputSize().y,
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, 0u
		);
	}

	// Initialize our linear render graph here
	// CREATE A LOT OF MEMORY LEAKS HERE
	// TODO: clean it up
	vector<Texture2D*>* simpleForwardSource = new vector<Texture2D*>();
	vector<Texture2D*>* simpleForwardSink = new vector<Texture2D*>();
	simpleForwardSink->push_back(hdrSceneRenderTarget[0].get());
	simpleForwardPass = new Passes::SimpleForwardPass(deviceResources, *simpleForwardSource, *simpleForwardSink);
	vector<Texture2D*>* postProcessingSource = new vector<Texture2D*>();
	vector<Texture2D*>* postProcessingSink = new vector<Texture2D*>();
	postProcessingSource->push_back(hdrSceneRenderTarget[0].get());
	postProcessingPass = new Passes::PostProcessingPass(deviceResources, *postProcessingSource, *postProcessingSink);

	OutputDebugString(L"Core Renderer ready \n");
}

void CoreRenderPipeline::Render()
{
	StartGUIFrame();

	camera->Update();

	// at each frame, the functions stored in the render graph, also known as passes, execute one by one.
	ShadowPass();
	simpleForwardPass->AddResources
	(
		Scene::Instance()->ObjectsInScene, camera.get(), 
		std::bind(&CoreRenderPipeline::UploadShadowResources, this),
		std::bind(&CoreRenderPipeline::UploadLightingResources, this)
	);
	(*simpleForwardPass)();
	(*postProcessingPass)();

	// Draw GUI on top of the game
	DrawGUI();
	deviceResources->Present();
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
	ImGui::DragFloat("Exposure", &exposure, 0.1f);

	ImGui::Checkbox("UseBloom", &useBloom);
	ImGui::DragFloat("Bloom Size", &bloomSize, 0.1f);
	ImGui::DragFloat("Bloom Threshold", &bloomThreshold, 0.001f);

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

void CoreRenderPipeline::UpdateWorld2Light()
{
	XMVECTOR position = XMLoadFloat3(&mainLightShadowCastingOrigin);
	XMVECTOR direction = XMLoadFloat3(&lights.MainLight.Direction);
	XMFLOAT3 upFloat3 = XMFLOAT3{ 0.0f, 1.0f, 0.0f };
	XMVECTOR up = XMLoadFloat3(&upFloat3);

	world2light = XMMatrixLookToLH(position, direction, up);
}

void CoreRenderPipeline::UploadShadowResources()
{
	// Upload shadow map to GPU
	winrt::com_ptr<ID3D11ShaderResourceView> shadowMapSRV = mainShadowMap->UseAsShaderResource();
	ID3D11ShaderResourceView* shadowMapSRVAddress = shadowMapSRV.get();
	deviceResources->Context()->PSSetShaderResources(mainShadowMap->Slot(), 1, &shadowMapSRVAddress);
	// Upload shadow parameters to GPU
	ShadowCasterParametersBuffer.Update(XMMatrixTranspose(world2light * shadowCaster.Perspective()));
	ShadowCasterParametersBuffer.Bind();
}

void CoreRenderPipeline::UploadLightingResources()
{
	// Update & bind all the lights
	lightsConstantBuffer->Update(lights);
	lightsConstantBuffer->Bind();
}

void CoreRenderPipeline::ShadowPass()
{
	deviceResources->ResetDefaultPipelineStates();
	deviceResources->Context()->ClearDepthStencilView
	(
		mainShadowMap->UseAsDepthStencil().get(), D3D11_CLEAR_DEPTH, 1.0f, 0
	);
	deviceResources->Context()->RSSetViewports(1, &shadowViewPort);
	deviceResources->SetTargets(0, nullptr, mainShadowMap->UseAsDepthStencil().get());

	// Render from the perspective of the main light
	UpdateWorld2Light();
	shadowCaster.Bind(world2light);

	for (GameObject*& gameObject : Scene::Instance()->ObjectsInScene)
	{
		gameObject->Render();
	}
}
