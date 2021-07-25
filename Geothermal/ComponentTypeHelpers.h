#pragma once

#include "ComponentPool.h"
#include "T_DummyComponent.h"

namespace ECS
{
	/// <summary>
	/// Shorthand of getting a component pool, as component types are currently recognized by their factory instances.
	/// </summary>
	/// <typeparam name="C">type of component</typeparam>
	/// <returns>ComponentPool*: pointer to the factory instance of requested type</returns>
	template <class C>
	ComponentPool<C>* TypeOf()
	{
		return ComponentPool<C>::GetInstance();
	}
}