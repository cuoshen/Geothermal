#include "pch.h"
#include "GameMain.h"

using namespace std;

GameMain* GameMain::instance;

GameMain::GameMain(shared_ptr<DeviceResources> device) :
	windowClosed(false), deltaTime(0.0f), time(0.0f), timer(nullptr), input(nullptr)
{
	if (device)
	{
		this->device = device;
		coreRenderer = make_unique<CoreRenderPipeline>(device);
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

void GameMain::Initialize(shared_ptr<DeviceResources> device)
{
	GameMain::instance = new GameMain(device);
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
	XMMATRIX initialTransform = XMMatrixTranslation(0.0f, 0.0f, 4.0f);
	AddDebugGameObject(initialTransform);
}

void GameMain::AddDebugGameObject(XMMATRIX initialTransform)
{
}

#endif