#pragma once

#include <vector>
#include <functional>

#include "Defs.h"
#include "Signiture.h"

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
		static void RegsiterEntityDestoryCallback(std::function<void(Entity)> newCallback);

		// entity modify event
		static void SignalEntityModifyEvent(Entity entity, Archetype newSigniture);
		static void RegsiterEntityModifyCallback(std::function<void(Entity, Archetype)> newCallback);

	private:
		static std::vector<std::function<void(Entity)>> EntityDestroyCallbacks;
		static std::vector<std::function<void(Entity, Archetype)>> EntityModifyCallbacks;
	};
}

