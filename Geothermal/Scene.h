#pragma once
#include "GameObject.h"
#include <list>

namespace Geothermal
{
	/// <summary>
	/// A scene is responsible for game object management
	/// </summary>
	class Scene
	{
		// TODO: Support multiple scenes
	public:
		static Scene* Instance()
		{
			if (instance == nullptr)
			{
				instance = new Scene();
			}
			return instance;
		}

	private:
		static Scene* instance;
		Scene() {};
	};
}
