#include "pch.h"
#include <d3dcompiler.h>
#include "CoreRenderPipeline.h"
#include "GraphicResources.h"
#include "ShaderCache.h"
#include "Material.h"
#include "GameMain.h"
#include "SceneManager.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

using namespace Geothermal;
using namespace Graphics;
using namespace SceneManagement;
using namespace Bindables;
using namespace Structures;
using namespace Passes;
using namespace std;
using namespace DirectX;

CoreRenderPipeline::CoreRenderPipeline(shared_ptr<DeviceResources> const& deviceResources) :
	RenderPipeline(deviceResources), 
	shadowParametersVSBuffer(deviceResources, ShadowMapSlot),
	shadowParametersPSBuffer(deviceResources, ShadowMapSlot),
	lights(DirectionalLight{ {1.0f, 1.0f, 1.0f, 1.0f}, {0.2f, -1.0f, 1.0f}, 0.0f }), debugMode(false)
{
	ShaderCache::Initialize(deviceResources);
	camera = make_unique<Camera>(deviceResources->AspectRatio(), 0.1f, 1000.0f, deviceResources);
	lightsConstantBuffer = make_unique<PixelConstantBuffer<ForwardLightBuffer>>(deviceResources, lights, 7);

	InitializeHDRTargets();
	InitializeGBuffers();
	BuildRenderGraph();

	OutputDebugString(L"Core render pipeline ready \n");
}

void CoreRenderPipeline::Render()
{
	StartGUIFrame();

	camera->Update();

	// at each frame, execute the passes one by one.

	XMVECTOR mainLightDirection = XMLoadFloat3(&lights.MainLight.Direction);
	world2light = shadowPass->UpdateWorld2Light(mainLightShadowCastingOrigin, mainLightDirection);
	(*shadowPass)();

	if (DeferredMode)
	{
		deferredGBufferPass->SetSceneResources(SceneManager::Instance().GameObjectRegistry, camera.get());
		(*deferredGBufferPass)();

		deferredLightingPass->SetDelegates(std::bind(&CoreRenderPipeline::UploadShadowResources, this));
		deferredLightingPass->SetParameters(deferredAmbience, lights.MainLight, camera.get());
		(*deferredLightingPass)();
	}
	else
	{
		simpleForwardPass->SetSceneResources(SceneManager::Instance().GameObjectRegistry, camera.get());
		simpleForwardPass->SetDelegates
		(
			std::bind(&CoreRenderPipeline::UploadShadowResources, this),
			std::bind(&CoreRenderPipeline::UploadLightingResources, this)
		);
		(*simpleForwardPass)();
	}

	(*postProcessingPass)();

	if (debugMode)
	{
		debugPass->SetSceneResources(SceneManager::Instance().GameObjectRegistry, camera.get());
		(*debugPass)();
	}

	DrawGUI();	// Draw GUI on top of game graphics
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
	XMVECTOR pos = camera->GetTransform().WorldPosition();
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
	if (ImGui::Button("Debug mode"))
	{
		debugMode = !debugMode;
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void CoreRenderPipeline::ResetCamera()
{
	camera->GetTransform().UpdateTransform(XMMatrixIdentity());
	camera->Pitch(0.0f);
	camera->Yaw(0.0f);
}

void CoreRenderPipeline::InitializeHDRTargets()
{
	for (u32 i = 0; i < HDRTargetCount; i++)
	{
		hdrTargets[i] = make_unique<Texture2D>
			(
				deviceResources, DXGI_FORMAT_R32G32B32A32_FLOAT,
				deviceResources->OutputSize().x, deviceResources->OutputSize().y,
				D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, 0u
				);
	}
}

void CoreRenderPipeline::InitializeGBuffers()
{
	gBuffers[0] = make_unique<Texture2D>
		(
			deviceResources, DXGI_FORMAT_R8G8B8A8_UNORM,
			deviceResources->OutputSize().x, deviceResources->OutputSize().y,
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, 0u
			);
	gBuffers[1] = make_unique<Texture2D>
		(
			deviceResources, DXGI_FORMAT_R32G32B32A32_FLOAT,
			deviceResources->OutputSize().x, deviceResources->OutputSize().y,
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, 1u
			);
	gBuffers[2] = make_unique<Texture2D>
		(
			deviceResources, DXGI_FORMAT_R32_TYPELESS,
			deviceResources->OutputSize().x, deviceResources->OutputSize().y,
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL, 2u
			);
}

void CoreRenderPipeline::BuildRenderGraph()
{
	shadowPass = make_unique<Passes::ShadowPass>(deviceResources);

	auto simpleForwardSink = make_unique<vector<Texture2D*>>();
	simpleForwardSink->push_back(hdrTargets[0].get());
	simpleForwardPass = make_unique<Passes::SimpleForwardPass>(deviceResources, nullptr, move(simpleForwardSink));

	auto gBufferSink = make_unique<vector<Texture2D*>>();
	for (u32 i = 0; i < GBufferCount; i++)
	{
		gBufferSink->push_back(gBuffers[i].get());
	}
	deferredGBufferPass = make_unique<Passes::DeferredGBufferPass>(deviceResources, move(gBufferSink));

	auto deferredLitSink = make_unique<vector<Texture2D*>>();
	deferredLitSink->push_back(hdrTargets[0].get());
	auto gBufferSource = make_unique<vector<Texture2D*>>();
	for (u32 i = 0; i < GBufferCount; i++)
	{
		gBufferSource->push_back(gBuffers[i].get());
	}
	deferredLightingPass = make_unique<Passes::DeferredLightingPass>(deviceResources, move(gBufferSource), move(deferredLitSink));

	auto postProcessingSource = make_unique<vector<Texture2D*>>();
	postProcessingSource->push_back(hdrTargets[0].get());
	postProcessingPass = make_unique<Passes::PostProcessingPass>(deviceResources, move(postProcessingSource), nullptr);

	debugPass = make_unique<Passes::WireframeDebugPass>(deviceResources);

	mainShadowMap = shadowPass->MainShadowMap();
}

void CoreRenderPipeline::UploadShadowResources()
{
	// Upload shadow map to GPU
	winrt::com_ptr<ID3D11ShaderResourceView> shadowMapSRV = mainShadowMap->UseAsShaderResource();
	ID3D11ShaderResourceView* shadowMapSRVAddress = shadowMapSRV.get();
	deviceResources->Context()->PSSetShaderResources(mainShadowMap->Slot(), 1, &shadowMapSRVAddress);
	// Upload shadow parameters to GPU
	shadowParametersVSBuffer.Update(XMMatrixTranspose(world2light * shadowPass->CasterPerspective()));
	shadowParametersVSBuffer.Bind();
	shadowParametersPSBuffer.Update(XMMatrixTranspose(world2light * shadowPass->CasterPerspective()));
	shadowParametersPSBuffer.Bind();
}

void CoreRenderPipeline::UploadLightingResources()
{
	// Update & bind all the lights
	lightsConstantBuffer->Update(lights);
	lightsConstantBuffer->Bind();
}
