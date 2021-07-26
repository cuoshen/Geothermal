#pragma once

// TODO: restrict the template type to derived classes of the base component

#include <vector>
#include "T_DummyComponent.h"
#include "ComponentPoolBase.h"
#include "EntityManager.h"

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
	public: /* methods */
		/// <summary>
		/// The singleton instance accessor.
		/// </summary>
		static ComponentPool<C>* GetInstance()
		{
			static MemGuard guard;
			// initialize if not already
			if (m_instance == nullptr)
			{
				m_instance = new ComponentPool<C>();
			}

			return m_instance;
		}

		/// <summary>
		/// allocate and return a new, initialized component of this type
		/// </summary>
		/// <returns></returns>
		C* NewComponent()
		{
			return nullptr;
		}

		/// <summary>
		/// remove the given component
		/// </summary>
		/// <param name="removee">component to be removed</param>
		void DeleteComponent(C* removee)
		{

		}

		int GetType() const override
		{
			return m_type;
		}

	private: /* fields */

		/// <summary>
		/// The singleton instance pointer.
		/// </summary>
		inline static ComponentPool<C>* m_instance;

		/// <summary>
		/// Storage of components.
		/// </summary>
		/// <typeparam name="C">component type</typeparam>
		std::vector<C> m_storage;

		int m_type;

		/* methods */
		
		/// <summary>
		/// The singleton constructor.
		/// I'm making a potentially erroneous move here: all component pools request their types on creation.
		/// </summary>
		ComponentPool<C>()
		{
			// check if the requested type derives from RuntimeComponent
			static_assert(std::is_base_of<RuntimeComponent, C>::value, "Component pool can only be used for subclasses of ECS::RuntimeComponent!");

			// initialize the buffer to 1000 components
			m_storage = std::vector<C>(1000);

			// request type id
			m_type = ComponentPoolBase::RequestTypeID(this);
		}

		class MemGuard
		{
		public:
			~MemGuard()
			{
				delete m_instance;
				m_instance = nullptr;
			}
		};
	};
}

