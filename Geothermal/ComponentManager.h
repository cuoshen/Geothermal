#pragma once

#include <typeinfo>
#include <vector>

#include "Defs.h"
#include "ComponentPool.h"

namespace ECS
{
	class Runtime;

	/// <summary>
	/// Outer interface for component pool usages.
	/// Not static, not singleton, just a normal, user-friendly object class.
	/// </summary>
	class ComponentManager
	{
	public:
		ComponentManager(Runtime* runtime);

		/// <summary>
		/// Create new component for an Entity.
		/// </summary>
		/// <typeparam name="T">type of component</typeparam>
		/// <param name="owner">entity to hold the component</param>
		/// <returns>a reference to the component</returns>
		template <class T>
		T& NewComponent(Entity owner)
		{
			T& outref = NewComponent_Internal<T>(owner);

			EntitySignalProxy(owner);

			return outref;
		}

		/// <summary>
		/// Create component with pre-determined value for entity.
		/// </summary>
		/// <typeparam name="T">type of component</typeparam>
		/// <param name="owner">entity to hold the component</param>
		/// <param name="newData">the data the new component has</param>
		/// <returns></returns>
		template <class T>
		T NewComponent(Entity owner, T newData)
		{
			T& outref = NewComponent<T>(owner);
			outref = newData;

			return outref;
		}

		template <class T>
		void RemoveComponent(Entity owner)
		{
			int type = ComponentPool<T>::TypeNum;

			if (Pools[type] != nullptr)
			{
				std::static_pointer_cast<ComponentPool<T>>(Pools[type])->Remove(owner);
			}
		}

		void OnEntityDestroy(Entity e);

	private:
		/// <summary>
		/// Create new component for an Entity.
		/// Not exposed to user.
		/// </summary>
		/// <typeparam name="T">type of component</typeparam>
		/// <param name="owner">entity to hold the component</param>
		/// <returns>a reference to the component</returns>
		template <class T>
		T& NewComponent_Internal(Entity owner)
		{
			int type = ComponentPool<T>::TypeNum;

			// late initialization of pool
			if (Pools[type] == nullptr)
			{
				Pools[type] = std::make_shared<ComponentPool<T>>();

				// add a callback function for destroy event
				OnEntityDestroyCallbacks[type] = [this](Entity e)
				{
					this->RemoveComponent<T>(e);
				};
			}

			// TODO: the exception workflow doens't make sense
			T& outref = std::static_pointer_cast<ComponentPool<T>>(Pools[type])->Insert(T(), owner);

			return outref;
		}

		void EntitySignalProxy(Entity e);

		Runtime* runtime;

		// These vectors are created with the right size calculated statically. Vector not array because it's not a static number.
		std::vector<std::shared_ptr<IComponentPool>> Pools = std::vector<std::shared_ptr<IComponentPool>>(IComponentPool::GetTypeCount());
		std::vector<std::function<void(Entity)>> OnEntityDestroyCallbacks = std::vector<std::function<void(Entity)>>(IComponentPool::GetTypeCount());
	};
}
