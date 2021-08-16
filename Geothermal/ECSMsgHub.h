#pragma once

#include "Defs.h"
#include <vector>
#include <functional>

namespace ECS
{
	/// <summary>
	/// Used for messaging internally between each logical module of ECS.
	/// Intentionally made very granular to avoid complex hierarchy.
	/// There doesn't seem to be any reason for this message hub to be a singleton, since it's not implementing any OOP feature.
	/// I copied this pattern from Unreal Insights.
	/// </summary>
	class ECSMsgHub
	{
	public:
		// entity destroy event
		static void SignalEntityDestoryEvent(Entity entity);
		static void RegsiterEntityDestoryCallback(std::function<void(Entity)> newCallback)
		{
			EntityDestroyCallbacks.push_back(newCallback);
		}

		// TODO: new entity event

		// TODO: component destroy event
		static void SignalComponentDestroyEvent(Entity owner); // TODO: is there need to also broadcast the component type ?

		// TODO: new component event
		static void SignalNewComponentEvent(Entity owner);

	private:
		static std::vector<std::function<void(Entity)>> EntityDestroyCallbacks;
	};
}

