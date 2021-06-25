#include "pch.h"
#include "GameMain.h"

using namespace std;

GameMain* GameMain::instance;

GameMain::GameMain(shared_ptr<DeviceResources> device) :
	windowClosed(false), deltaTime(0.0f), time(0.0f), timer(nullptr)
	//, input(nullptr)
{
	if (device)
	{
		this->device = device;
		coreRenderer = make_unique<CoreRenderPipeline>(device);
		OutputDebugString(L"GameMain created \n");
	}
	timer = make_unique<GameTimer>();
	timer->Start();
	//input = make_unique<Input>(CoreWindow::GetForCurrentThread());
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
	//input->Update();
}
