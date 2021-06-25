#pragma once
#include "DeviceResources.h"
//#include "CoreRenderPipeline.h"
//#include "GameTimer.h"
//#include "Input.h"
//#include "Scene.h"

using namespace Aeriel;
using namespace Aeriel::Graphics;

/// <summary>
/// Main gameplay loop
/// Singleton class that must be explicitly initialized once
/// </summary>
class GameMain
{
public:
	static GameMain* GetInstance();
	static void Initialize(std::shared_ptr<DeviceResources> device);

	void Run();
	void Update();
	void LateUpdate();
	void Close() { windowClosed = true; }

	float GetDeltaTime() const { return deltaTime; }
	float GetTime() const { return time; }
	//Aeriel::Input* GetInput() const { return input.get(); }
private:
	GameMain(std::shared_ptr<DeviceResources> device);
	static GameMain* instance;

	bool windowClosed;
	//std::unique_ptr<GameTimer> timer;
	float time;
	float deltaTime;
	//std::unique_ptr<Aeriel::Input> input;
	std::shared_ptr<DeviceResources> device;
	//std::unique_ptr<CoreRenderPipeline> coreRenderer;
};