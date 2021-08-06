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
	lights(DirectionalLight{ {1.0f, 1.0f, 1.0f, 1.0f}, {0.2f, -1.0f, 1.0f}, 0.0f }),
	ShadowCasterParametersBuffer(deviceResources, 5u), mainShadowMap(nullptr)
{
	ShaderCache::Initialize(deviceResources);

	camera = make_unique<Camera>(deviceResources->AspectRatio(), 0.1f, 1000.0f, deviceResources);
	lightsConstantBuffer = make_unique<PixelConstantBuffer<LightBuffer>>(deviceResources, lights, 7);
	
	for (int i = 0; i < 4; i++)
	{
		hdrTargets[i] = make_unique<Texture2D>
		(
		deviceResources, DXGI_FORMAT_R32G32B32A32_FLOAT,
		deviceResources->OutputSize().x, deviceResources->OutputSize().y,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, 0u
		);
	}

	// Initialize our linear render graph here
	// We create sources and sinks here, then pass their control into the RenderPasses
	// No memory leak should occur

	shadowPass = new Passes::ShadowPass(deviceResources, {}, {});

	vector<Texture2D*>* simpleForwardSink = new vector<Texture2D*>();
	simpleForwardSink->push_back(hdrTargets[0].get());
	simpleForwardPass = new Passes::SimpleForwardPass(deviceResources, {}, *simpleForwardSink);

	vector<Texture2D*>* postProcessingSource = new vector<Texture2D*>();
	postProcessingSource->push_back(hdrTargets[0].get());
	postProcessingPass = new Passes::PostProcessingPass(deviceResources, *postProcessingSource, {});

	mainShadowMap = shadowPass->MainShadowMap();

	OutputDebugString(L"Core Renderer ready \n");
}

void CoreRenderPipeline::Render()
{
	StartGUIFrame();

	camera->Update();

	// at each frame, execute the passes one by one.

	XMVECTOR mainLightDirection = XMLoadFloat3(&lights.MainLight.Direction);
	world2light = shadowPass->UpdateWorld2Light(mainLightShadowCastingOrigin, mainLightDirection);
	(*shadowPass)();

	simpleForwardPass->SetResources
	(
		Scene::Instance()->ObjectsInScene, camera.get(), 
		std::bind(&CoreRenderPipeline::UploadShadowResources, this),
		std::bind(&CoreRenderPipeline::UploadLightingResources, this)
	);
	(*simpleForwardPass)();

	(*postProcessingPass)();

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

void CoreRenderPipeline::UploadShadowResources()
{
	// Upload shadow map to GPU
	winrt::com_ptr<ID3D11ShaderResourceView> shadowMapSRV = mainShadowMap->UseAsShaderResource();
	ID3D11ShaderResourceView* shadowMapSRVAddress = shadowMapSRV.get();
	deviceResources->Context()->PSSetShaderResources(mainShadowMap->Slot(), 1, &shadowMapSRVAddress);
	// Upload shadow parameters to GPU
	ShadowCasterParametersBuffer.Update(XMMatrixTranspose(world2light * shadowPass->CasterPerspective()));
	ShadowCasterParametersBuffer.Bind();
}

void CoreRenderPipeline::UploadLightingResources()
{
	// Update & bind all the lights
	lightsConstantBuffer->Update(lights);
	lightsConstantBuffer->Bind();
}
