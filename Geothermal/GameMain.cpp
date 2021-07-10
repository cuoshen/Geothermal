#include "pch.h"
#include "GameMain.h"

#ifdef DEBUG_SCENE
#include "ModelLoader.h"
#include "Mesh.h"
#include "ConstantBuffer.h"
#include "SamplerState.h"
#include "Texture2D.h"
using namespace Geothermal;
using namespace Graphics;
using namespace Bindables;
using namespace Structures;
using namespace Meshes;
#endif

using namespace std;

GameMain* GameMain::instance;

GameMain::GameMain(shared_ptr<DeviceResources> deviceResources) :
	windowClosed(false), deltaTime(0.0f), time(0.0f), timer(nullptr), input(nullptr)
{
	if (deviceResources)
	{
		this->deviceResources = deviceResources;
		coreRenderer = make_unique<CoreRenderPipeline>(deviceResources);
		OutputDebugString(L"GameMain created \n");
	}
	timer = make_unique<GameTimer>();
	timer->Start();
	input = make_unique<Input>();
}

GameMain* GameMain::Instance()
{
	assert(GameMain::instance != nullptr);
	return GameMain::instance;
}

void GameMain::Initialize(shared_ptr<DeviceResources> deviceResources)
{
	GameMain::instance = new GameMain(deviceResources);

#ifdef DEBUG_SCENE
	GameMain::instance->LoadDebugMesh();
	GameMain::instance->InstantiateDebugScene();
#endif
}

UINT GameMain::HandleMessage(MSG msg)
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

/// <summary>
/// Runs the main game of Update-Render until quit
/// </summary>
/// <returns>Exit state</returns>
WPARAM GameMain::Run()
{
	MSG msg = { 0 };
	while (!windowClosed)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) > 0)
		{
			UINT result = HandleMessage(msg);
			if (result != 0)
			{
				return msg.wParam;
			}
		}
		Update();
		LateUpdate();
	}

	return msg.wParam;
}

/// <summary>
/// Update function is called once per frame, before the frame is rendered
/// </summary>
void GameMain::Update()
{
	timer->Update();
	deltaTime = timer->DeltaTime();
	time = timer->PlayingTime();

	for (GameObject*& gameObject : Scene::Instance()->ObjectsInScene)
	{
		gameObject->Update();

#ifdef DEBUG_SCENE
		// Self-rotate
		XMVECTOR position = gameObject->GetTransform()->WorldPosition();
		gameObject->GetTransform()->ApplyTransform
		(
			XMMatrixTranslationFromVector(-position) * 
			XMMatrixRotationY(deltaTime) * 
			XMMatrixTranslationFromVector(position)
		);
#endif
	}

	coreRenderer->Render();
}

/// <summary>
/// Update after the frame is rendered
/// </summary>
void GameMain::LateUpdate()
{
	input->Update();
}

#ifdef DEBUG_SCENE

void GameMain::InstantiateDebugScene()
{
	XMMATRIX initialTransform = XMMatrixRotationX(XM_PI) * XMMatrixTranslation(0.0f, 0.0f, 4.0f) ;
	AddDebugGameObject(initialTransform);
}

void GameMain::LoadDebugMesh()
{
	ModelLoader loader;
	debugMesh = new Mesh();
	bool loaded =
		//loader.LoadObj2Mesh(L"Assets\\stanford_dragon.obj", L"Assets\\stanford_dragon.mtl", debugMesh, deviceResources);
		loader.LoadObj2Mesh(L"Assets\\smooth_sphere.obj", L"Assets\\smooth_sphere.mtl", debugMesh, deviceResources);
	assert(loaded);
	shadingParameters = PhongAttributes
	{
		{0.0f, 0.0f, 0.06f, 0.0f},	// Ambient
		{0.1f, 0.1f, 0.1f, 1.0f},		// Base color
		0.5f,										// Diffuse
		0.5f,										// Specular
		20.0f,										// Smoothness
		0.0f											// Padding
	};
	PixelConstantBuffer<PhongAttributes> unlitProperties(deviceResources, shadingParameters, 2u);
	unlitProperties.Bind();

	SamplerState samplerState(deviceResources);
	samplerState.Bind();

	Texture2D debugAlbedoTexture(deviceResources, L"Assets\\earth.dds", DDS);
	winrt::com_ptr<ID3D11ShaderResourceView> albedoAsSRV = debugAlbedoTexture.UseAsShaderResource();
	ID3D11ShaderResourceView* albedoSRVAddress = albedoAsSRV.get();
	deviceResources->D3DDeviceContext()->PSSetShaderResources(0, 1, &albedoSRVAddress);

	Texture2D debugNormalTexture(deviceResources, L"Assets\\concrete_normal.dds", DDS);
	winrt::com_ptr<ID3D11ShaderResourceView> normalAsSRV = debugNormalTexture.UseAsShaderResource();
	ID3D11ShaderResourceView* normalSRVAddress = normalAsSRV.get();
	deviceResources->D3DDeviceContext()->PSSetShaderResources(1, 1, &normalSRVAddress);
}

void GameMain::AddDebugGameObject(XMMATRIX initialTransform)
{
	GameObjectFactory factory;
	factory.MakeNewProduct();
	factory.BuildTransform(initialTransform);
	factory.BuildRenderer(*debugMesh, deviceResources); // Use the debug mesh
	factory.SetObjectID(0x01);
	shared_ptr<GameObject> product = factory.GetProduct();	// Register to main scene by default

	debugGameObjects.push_back(product);
}

#endif