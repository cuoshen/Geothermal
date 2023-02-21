#pragma once
#include "GameObject.h"
#include "Scene.h"

/// <summary>
/// Basic principles:
/// SceneManager manages multiple scenes, each contains DESCRIPTION of game objects, not actual objects themselves.
/// At runtime, there exists a single active scene. Only the game objects within the single scene is present.
/// When a scene deactivates, all game objects within the scene is cleared from memory.
/// </summary>
namespace Geothermal::SceneManagement
{
	/// <summary>
	/// Singleton scene manager that controls scene activation and game object registry
	/// </summary>
	class SceneManager
	{
	public:
		static SceneManager& Instance()
		{
			static SceneManager instance;
			return instance;
		}
		SceneManager(SceneManager const&) = delete;
		void operator=(SceneManager const&) = delete;

		Scene* ActiveScene() { return &(scenes[activeSceneIndex]); }
		std::list<GameObject*> GameObjectRegistry;

	private:
		SceneManager() : activeSceneIndex(0) {}		// Hide constructor

		std::vector<Scene> scenes;
		u32 activeSceneIndex;
	};
}
