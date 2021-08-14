#include "pch.h"
#include "GameMain.h"
#include "imgui_impl_win32.h"

#ifdef DEBUG_SCENE
#include "ModelLoader.h"
#include "Mesh.h"
#include "Material.h"
using namespace Geothermal;
using namespace Graphics;
using namespace Bindables;
using namespace Structures;
using namespace Meshes;
using namespace Materials;
#endif

using namespace std;
using namespace winrt;
using namespace DirectX;

GameMain* GameMain::instance;

GameMain::GameMain(shared_ptr<DeviceResources> deviceResources) :
	windowClosed(false), deltaTime(0.0f), time(0.0f), timer(nullptr), input(nullptr)
{
	UINT hardwareSupportedThreadNumber = thread::hardware_concurrency();
	wstring threadsInfo = to_wstring(hardwareSupportedThreadNumber) + wstring(L" concurrent threads are supported on this device.\n");
	OutputDebugString(threadsInfo.c_str());

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
	GameMain::instance->InitializeDebugResource();
	GameMain::instance->InstantiateDebugScene();
#endif
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

UINT GameMain::HandleMessage(MSG msg)
{
	// We don't really need the LRESULT from imgui
	ImGui_ImplWin32_WndProcHandler(msg.hwnd, msg.message, msg.wParam, msg.lParam);

	if (msg.message == WM_QUIT)
	{
		return 1;
	}

	TranslateMessage(&msg);
	DispatchMessage(&msg);

	GameMain::Instance()->GetInput()->RegisterInput(&msg);

	return 0;
}

/// <summary>
/// Runs the main game loop of Update-Render until quit
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

//#define SELF_ROTATE

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

#ifdef SELF_ROTATE
		if (gameObject != ground)
		{
			XMVECTOR position = gameObject->GetTransform()->WorldPosition();
			gameObject->GetTransform()->ApplyTransform
			(
				XMMatrixTranslationFromVector(-position) *
				XMMatrixRotationY(deltaTime) *
				XMMatrixTranslationFromVector(position)
			);
		}
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
	XMMATRIX center = XMMatrixTranslation(0.0f, 0.0f, 10.0f);
	for (int i = -1; i <= 1; i++)
	{
		XMMATRIX initialTransform = center * XMMatrixTranslation((float)i * 8.0f, 0.0f, 0.0f);
		AddDebugGameObject(initialTransform);
	}
	AddGround(center);
}

void GameMain::InitializeDebugResource()
{
	ModelLoader loader;
	debugMesh = new Mesh();
	bool loaded =
		loader.LoadObj2Mesh(L"Assets\\building_2.obj", L"Assets\\building_2.mtl", debugMesh, deviceResources);
	//loader.LoadObj2Mesh(L"Assets\\sphere.obj", L"Assets\\sphere.mtl", debugMesh, deviceResources);
	assert(loaded);

	debugPlane = new Mesh();
	loaded =
		loader.LoadObj2Mesh(L"Assets\\plane.obj", L"Assets\\plane.mtl", debugPlane, deviceResources);
	assert(loaded);

	// For now we want to put everything into a single test material
	// TODO: remove this and support per-object material instead
	materials[0] =
		make_shared<Material>(deviceResources, L"LitVertexShader.cso", L"ForwardLit.cso", VertexPNTTLayout, (uint)size(VertexPNTTLayout));
	materials[1] =
		make_shared<Material>(deviceResources, L"LitVertexShader.cso", L"ForwardLit.cso", VertexPNTTLayout, (uint)size(VertexPNTTLayout));

	ShadingAttributes shadingParameters0 = ShadingAttributes
	{
		{0.0f, 0.0f, 0.06f, 0.0f},											// Ambient
		{0.8f, 0.8f, 0.8f, 1.0f},												// Base color
		0.2f,																				// Diffuse
		2.0f,																				// Specular
		10.0f,																				// Smoothness
		USE_SHADOW_MAP		// Texture flags
	};

	shared_ptr<PixelConstantBuffer<ShadingAttributes>> properties0 =
		make_shared<PixelConstantBuffer<ShadingAttributes>>(deviceResources, shadingParameters0, 2u);
	materials[0]->AddParameterSet(properties0);

	ShadingAttributes shadingParameters1 = ShadingAttributes
	{
		{0.0f, 0.0f, 0.06f, 0.0f},											// Ambient
		{0.3f, 0.3f, 0.3f, 1.0f},												// Base color
		0.5f,																				// Diffuse
		0.5f,																				// Specular
		3.0f,																				// Smoothness
		USE_SHADOW_MAP | USE_ALBEDO_MAP | USE_NORMAL_MAP		// Texture flags
	};

	shared_ptr<PixelConstantBuffer<ShadingAttributes>> properties1 =
		make_shared<PixelConstantBuffer<ShadingAttributes>>(deviceResources, shadingParameters1, 2u);
	materials[1]->AddParameterSet(properties1);

	SamplerState samplerState(deviceResources);
	samplerState.Bind();

	shared_ptr<Texture2D> debugAlbedoTexture0 =
		make_shared<Texture2D>(deviceResources, L"Assets\\concrete_albedo.dds", TEXTURE_FILE_TYPE::DDS, 0u);
	materials[0]->AddTexture(debugAlbedoTexture0);
	shared_ptr<Texture2D> debugAlbedoTexture1 =
		make_shared<Texture2D>(deviceResources, L"Assets\\paint_albedo.dds", TEXTURE_FILE_TYPE::DDS, 0u);
	materials[1]->AddTexture(debugAlbedoTexture1);

	shared_ptr<Texture2D> debugNormalTexture0 =
		make_shared<Texture2D>(deviceResources, L"Assets\\concrete_normal.dds", TEXTURE_FILE_TYPE::DDS, 1u);
	materials[0]->AddTexture(debugNormalTexture0);
	shared_ptr<Texture2D> debugNormalTexture1 =
		make_shared<Texture2D>(deviceResources, L"Assets\\paint_normal.dds", TEXTURE_FILE_TYPE::DDS, 1u);
	materials[1]->AddTexture(debugNormalTexture1);
}

void GameMain::AddDebugGameObject(XMMATRIX initialTransform)
{
	GameObjectFactory factory;
	factory.MakeNewProduct();
	factory.BuildTransform(initialTransform);
	factory.BuildRenderer(deviceResources, *debugMesh, materials[0]); // Use the debug mesh
	factory.SetObjectID(0x01);
	shared_ptr<GameObject> product = factory.GetProduct();	// Register to main scene by default

	debugGameObjects.push_back(product);
}

void GameMain::AddGround(XMMATRIX initialTransform)
{
	GameObjectFactory factory;
	factory.MakeNewProduct();
	factory.BuildTransform(initialTransform);
	factory.BuildRenderer(deviceResources, *debugPlane, materials[1]); // Use the debug mesh
	factory.SetObjectID(0x01);
	shared_ptr<GameObject> product = factory.GetProduct();	// Register to main scene by default

	ground = product.get();
	debugGameObjects.push_back(product);
}

#endif