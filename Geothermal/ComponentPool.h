#pragma once

// TODO: restrict the template type to derived classes of the base component

namespace ECS 
{
	/// <summary>
	/// a managed collection of components of the same type
	/// </summary>
	/// <typeparam name="C">component type</typeparam>
	template <typename C>
	class ComponentPool
	{
	public:
		/// <summary>
		/// allocate and return a new, initialized component of this type
		/// </summary>
		/// <returns></returns>
		C* NewComponent();

		/// <summary>
		/// remove the given component
		/// </summary>
		/// <param name="removee">component to be removed</param>
		void DeleteComponent(C* removee);
	};
}

