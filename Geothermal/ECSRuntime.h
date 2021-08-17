#pragma once

#include <vector>
#include <functional>

#include "Defs.h"

namespace ECS
{
	struct Archetype;

	class Runtime
	{
	public:
		Runtime();

	private:
		friend class EntityManager;
		friend class ComponentManager;
		friend class SystemManager;

		//void SignalEntityDestroyEvent(Entity entity);
		//void SignalEntityModifyEvent(Entity entity, Archetype signiture);

		std::shared_ptr<EntityManager> entityManager;
		std::shared_ptr<ComponentManager> componentManager;
		std::shared_ptr<SystemManager> systemManager;

		//// message tunnels
		//std::vector<std::function<void(Entity)>> EntityDestroyCallbacks;
		//std::vector<std::function<void(Entity, Archetype)>> EntityModifyCallbacks;
	};
}

