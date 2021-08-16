#pragma once

#include <typeinfo>
#include <vector>

#include "Defs.h"
#include "ECSMsgHub.h"
#include "ComponentPool.h"

namespace ECS
{
	/// <summary>
	/// Outer interface for component pool usages.
	/// Not static, not singleton, just a normal, user-friendly object class.
	/// </summary>
	class ComponentManager
	{
	public:
		ComponentManager()
		{
			Pools = std::vector<std::shared_ptr<IComponentPool>>(IComponentPool::GetTypeCount());
		}

		/// <summary>
		/// Create new component for an Entity.
		/// </summary>
		/// <typeparam name="T">type of component</typeparam>
		/// <param name="owner">entity to hold the component</param>
		/// <returns>a reference to the component</returns>
		template <class T>
		T& NewComponent(Entity owner)
		{
			int type = ComponentPool<T>::TypeNum;

			if (Pools[type] == nullptr)
			{
				Pools[type] = std::make_shared<ComponentPool<T>>();
			}

			T& outref = std::static_pointer_cast<ComponentPool<T>>(Pools[type])->Insert(T(), owner);

			ECSMsgHub::SignalNewComponentEvent(owner);
		
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

			}
		}

	private:
		/// <summary>
		/// This vector is created with the right size calculated statically.
		/// </summary>
		std::vector<std::shared_ptr<IComponentPool>> Pools;
	};
}
