#pragma once

#include <vector>
#include <functional>

#include "Defs.h"
#include "ComponentManager.h"
#include "System.h"
#include "EntityManager.h"

namespace Geothermal::ECS
{
	struct Archetype;

	class Runtime
	{
	public:
		Runtime() = default;

		// user interface ////////////////////////////
		template <class T>
		T& NewComponent(Entity e) 
		{
			T& outref = componentManager.NewComponent<T>(e);
			
			// update signiture
			entityManager.AddComponentSigniture<T>(e);

			// update system lists
			systemManager.OnSignitureChange(e, entityManager.GetSigniture(e));
		}

		template <class T>
		void RemoveComponent(Entity e) 
		{
			componentManager.RemoveComponent<T>(e);

			// update signiture
			entityManager.RemoveComponentSigniture<T>(e);

			// update lists
			systemManager.OnSignitureChange(e, entityManager.GetSigniture(e));
		}

		Entity NewEntity();

		Entity NewEntity(const Archetype& signiture);

		void DestroyEntity(Entity e);
		// end: user interface ///////////////////////

	private:
		EntityManager entityManager;
		ComponentManager componentManager;
		SystemManager systemManager;
	};
}

