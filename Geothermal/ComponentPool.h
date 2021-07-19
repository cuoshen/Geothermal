#pragma once

// TODO: restrict the template type to derived classes of the base component

#include <vector>
#include "T_DummyComponent.h"
#include "ComponentPoolBase.h"

namespace ECS 
{
	/// <summary>
	/// a managed collection of components of the same type
	/// It's a singleton, initialized lazily as an empty pool.
	/// For now I'm not allowing derivation of it since inheritance of this system class seems pointless and violates ECS principles.
	/// </summary>
	/// <typeparam name="C">component type</typeparam>
	template <class C>
	class ComponentPool final : public ComponentPoolBase
	{
	private: /* fields */
		/// <summary>
		/// The singleton instance pointer.
		/// </summary>
		static ComponentPool<C>* m_instance;

		/// <summary>
		/// Storage of components.
		/// </summary>
		/// <typeparam name="C">component type</typeparam>
		std::vector<C> m_storage;

		int m_type;
	
	public: /* methods */
		/// <summary>
		/// The singleton instance accessor.
		/// </summary>
		/// <returns></returns>
		static ComponentPool<C>* GetInstance();

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

		int GetType() override;


	private: /* methods */
		/// <summary>
		/// The singleton constructor.
		/// I'm making a potentially erroneous move here: all component pools request their types on creation.
		/// </summary>
		ComponentPool<C>();
	};

	// TODO: this might be erroneous
	template <class C>
	ComponentPool<C>* ComponentPool<C>::m_instance = nullptr;
}

