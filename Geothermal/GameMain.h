#pragma once
#include "GraphicResources.h"
#include "CoreRenderPipeline.h"
#include "GameTimer.h"
#include "Input.h"
#include "Scene.h"

#define DEBUG_SCENE

#ifdef DEBUG_SCENE
#include "Mesh.h"
#endif

using namespace Geothermal;
using namespace Geothermal::Graphics;

/// <summary>
/// Main gameplay loop
/// Singleton class that must be explicitly initialized once
/// </summary>
class GameMain
{
public:
	static GameMain* Instance();
	static void Initialize(std::shared_ptr<DeviceResources> deviceResources);

	WPARAM Run();
	void Update();
	void LateUpdate();
	void Close() { windowClosed = true; }

	float DeltaTime() const { return deltaTime; }
	float Time() const { return time; }
	Geothermal::Input* GetInput() const { return input.get(); }
private:
	GameMain(std::shared_ptr<DeviceResources> deviceResources);
	static GameMain* instance;

	UINT HandleMessage(MSG msg);

	bool windowClosed;
	std::unique_ptr<GameTimer> timer;
	float time;
	float deltaTime;
	std::unique_ptr<Geothermal::Input> input;
	std::shared_ptr<DeviceResources> deviceResources;
	std::unique_ptr<CoreRenderPipeline> coreRenderer;

#ifdef DEBUG_SCENE
	std::vector<std::shared_ptr<GameObject>> debugGameObjects;
	void InstantiateDebugScene();

	Meshes::Mesh* debugMesh;
	Meshes::Mesh* debugPlane;
	GameObject* ground;
	void InitializeDebugResource();

	void AddDebugGameObject(XMMATRIX initialTransform);
	void AddGround(XMMATRIX initialTransform);
#endif
};
