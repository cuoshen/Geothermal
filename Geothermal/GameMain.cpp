#include "pch.h"
#include "GameMain.h"

using namespace std;

GameMain* GameMain::instance;

GameMain::GameMain(shared_ptr<DeviceResources> device) :
	windowClosed(false), deltaTime(0.0f), time(0.0f)
	//, input(nullptr), timer(nullptr)
{
	if (device)
	{
		this->device = device;
		//coreRenderer = make_unique<CoreRenderPipeline>(device);
		OutputDebugString(L"GameMain created \n");
	}
	/*timer = make_unique<GameTimer>();
	timer->Start();
	input = make_unique<Input>(CoreWindow::GetForCurrentThread());*/
}

GameMain* GameMain::GetInstance()
{
	assert(GameMain::instance != nullptr);
	return GameMain::instance;
}

void GameMain::Initialize(shared_ptr<DeviceResources> device)
{
	GameMain::instance = new GameMain(device);
}

/// <summary>
/// Run function is called once, and run forever until the window is closed
/// </summary>
void GameMain::Run()
{
	OutputDebugString(L"Start game \n");
	while (!windowClosed)
	{
		Update();
		//coreRenderer->Render();
		LateUpdate();
	}
}

/// <summary>
/// Update function is called once per frame, before the frame is rendered
/// </summary>
void GameMain::Update()
{
	/*timer->Update();
	deltaTime = timer->DeltaTime();
	time = timer->PlayingTime();

	for (GameObject*& gameObject : Scene::GetInstance()->ObjectsInScene)
	{
		gameObject->Update();
	}*/
}

/// <summary>
/// Update after the frame is rendered
/// </summary>
void GameMain::LateUpdate()
{
	//input->Update();
}
