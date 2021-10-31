#pragma once
#include "GraphicResources.h"
#include "RenderPipeline.h"
#include "GameTimer.h"
#include "Input.h"
#include "Scene.h"

#define DEBUG_SCENE

#ifdef DEBUG_SCENE
#include "Mesh.h"
#endif

namespace Geothermal
{
	/// <summary>
	/// Main gameplay loop
	/// Singleton class that must be explicitly initialized once
	/// </summary>
	class GameMain
	{
	public:
		static GameMain* Instance();
		static void Initialize(std::shared_ptr<Graphics::DeviceResources> deviceResources);

		WPARAM Run();
		void Update();
		void LateUpdate();
		void Close() { windowClosed = true; }

		float DeltaTime() const { return deltaTime; }
		float Time() const { return time; }
		Input* GetInput() const { return input.get(); }

	private:
		GameMain(std::shared_ptr<Graphics::DeviceResources> deviceResources);
		static GameMain* instance;

		UINT HandleMessage(MSG msg);

		bool windowClosed;
		std::unique_ptr<GameTimer> timer;
		float time;
		float deltaTime;
		std::unique_ptr<Input> input;
		std::shared_ptr<Graphics::DeviceResources> deviceResources;
		std::unique_ptr<Graphics::RenderPipeline> renderPipeline;

#ifdef DEBUG_SCENE
		std::vector<std::shared_ptr<GameObject>> debugGameObjects;
		std::shared_ptr<Graphics::Materials::Material> materials[2];
		void InstantiateDebugScene();

		Graphics::Meshes::Mesh* debugMesh;
		Graphics::Meshes::Mesh* debugPlane;
		GameObject* ground;
		void InitializeDebugResource();

		void AddDebugGameObject(XMMATRIX initialTransform);
		void AddGround(XMMATRIX initialTransform);
#endif
	};
}

