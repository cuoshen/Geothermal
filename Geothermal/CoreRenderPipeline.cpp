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
using namespace std;
using namespace DirectX;

CoreRenderPipeline::CoreRenderPipeline(std::shared_ptr<DeviceResources> const& deviceResources) :
	deviceResources(deviceResources), camera(nullptr), lightConstantBuffer(nullptr),
	lights(DirectionalLight{ {1.0f, 1.0f, 1.0f, 1.0f}, {0.2f, -1.0f, 1.0f}, 0.0f }),
	shadowCaster(deviceResources, 30.0f, 30.0f, 0.0f, 1000.0f), bloomSize(5.0f), bloomBrightness(5.0f),
	ShadowCasterParametersBuffer(deviceResources, 5u), toneMapper(nullptr), exposure(0.0f),
	mainShadowMap(nullptr), basicPostProcess(nullptr), dualPostProcess(nullptr)
{
	ShaderCache::Initialize(deviceResources);

	camera = make_unique<Camera>(deviceResources->AspectRatio(), 0.1f, 1000.0f, deviceResources);
	lightConstantBuffer = make_unique<PixelConstantBuffer<LightBuffer>>(deviceResources, lights, 7);
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
	
	basicPostProcess = make_unique<BasicPostProcess>(deviceResources->Device());
	dualPostProcess = make_unique<DualPostProcess>(deviceResources->Device());
	toneMapper = make_unique<ToneMapPostProcess>(deviceResources->Device());

	// Initialize our linear render graph here
	// a linear render graph is a list of functions or passes, each performs a specific render operation

	linearRenderGraph.push_back(std::bind(&CoreRenderPipeline::ShadowPass, this));
	linearRenderGraph.push_back(std::bind(&CoreRenderPipeline::SimpleForwardPass, this));
	linearRenderGraph.push_back(std::bind(&CoreRenderPipeline::PostProcessingPass, this));

	OutputDebugString(L"Core Renderer ready \n");
}

void CoreRenderPipeline::Render()
{
	StartGUIFrame();

	camera->Update();

	// at each frame, the functions stored in the render graph, also known as passes, execute one by one.
	for (auto renderPass : linearRenderGraph)
	{
		renderPass();
	}

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
	ImGui::DragFloat("Bloom Size", &bloomSize, 0.1f);
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

void CoreRenderPipeline::SimpleForwardPass()
{
	deviceResources->ResetDefaultPipelineStates();
	deviceResources->Context()->ClearRenderTargetView
	(
		hdrSceneRenderTarget[0]->UseAsRenderTarget().get(), deviceResources->ClearColor
	);
	deviceResources->Context()->ClearDepthStencilView
	(
		deviceResources->DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0
	);
	deviceResources->Context()->RSSetViewports(1, &(deviceResources->ScreenViewport()));
	ID3D11RenderTargetView* target = hdrSceneRenderTarget[0]->UseAsRenderTarget().get();
	deviceResources->SetTargets(1, &target, deviceResources->DepthStencilView());

	camera->BindCamera2Pipeline();		// Render from the perspective of the main camera

	UploadShadowResources();

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

void CoreRenderPipeline::PostProcessingPass()
{
	ApplyBloom();

	// Clear back buffer and target it in OutputMerger
	deviceResources->ClearFrame();
	ID3D11RenderTargetView* target = deviceResources->BackBufferTargetView();
	deviceResources->SetTargets(1, &target, nullptr);

	toneMapper->SetOperator(ToneMapPostProcess::Reinhard);
	toneMapper->SetHDRSourceTexture(hdrSceneRenderTarget[1]->UseAsShaderResource().get());
	toneMapper->SetExposure(exposure);
	toneMapper->SetTransferFunction(ToneMapPostProcess::Linear);
	toneMapper->Process(deviceResources->Context());
}

void CoreRenderPipeline::ApplyBloom()
{
	// Clear both bloom targets
	for (int i = 0; i < 2; i++)
	{
		deviceResources->Context()->ClearRenderTargetView
		(
			bloomTextures[1]->UseAsRenderTarget().get(), deviceResources->ClearColor
		);
	}
	
	ID3D11RenderTargetView* target = bloomTextures[0]->UseAsRenderTarget().get();
	deviceResources->SetTargets(1, &target, nullptr);

	basicPostProcess->SetEffect(BasicPostProcess::BloomExtract);
	basicPostProcess->SetBloomExtractParameter(1.0f);
	basicPostProcess->SetSourceTexture(hdrSceneRenderTarget[0]->UseAsShaderResource().get());
	basicPostProcess->Process(deviceResources->Context());

	// Send blur to the other bloom texture
	target = bloomTextures[1]->UseAsRenderTarget().get();
	deviceResources->SetTargets(1, &target, nullptr);

	basicPostProcess->SetEffect(BasicPostProcess::BloomBlur);
	basicPostProcess->SetBloomBlurParameters(true, bloomSize, bloomBrightness);
	basicPostProcess->SetSourceTexture(bloomTextures[0]->UseAsShaderResource().get());
	basicPostProcess->Process(deviceResources->Context());

	// And then back
	target = bloomTextures[0]->UseAsRenderTarget().get();
	deviceResources->SetTargets(1, &target, nullptr);

	basicPostProcess->SetEffect(BasicPostProcess::BloomBlur);
	basicPostProcess->SetBloomBlurParameters(true, bloomSize, bloomBrightness);
	basicPostProcess->SetSourceTexture(bloomTextures[1]->UseAsShaderResource().get());
	basicPostProcess->Process(deviceResources->Context());

	// Finally combine to scene render buffer
	target = hdrSceneRenderTarget[1]->UseAsRenderTarget().get();
	deviceResources->SetTargets(1, &target, nullptr);

	dualPostProcess->SetEffect(DualPostProcess::BloomCombine);
	dualPostProcess->SetBloomCombineParameters(1.0f, 1.0f, 1.0f, 1.0f);
	dualPostProcess->SetSourceTexture(bloomTextures[0]->UseAsShaderResource().get());
	dualPostProcess->SetSourceTexture2(hdrSceneRenderTarget[0]->UseAsShaderResource().get());
	dualPostProcess->Process(deviceResources->Context());
}
