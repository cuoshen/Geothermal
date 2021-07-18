#include "ComponentPool.h"
#include "RuntimeComponent.h"

namespace ECS {
	template <class C>
	ComponentPool<C>* ComponentPool<C>::GetInstance() {
		// initialize if not already
		if (m_instance == nullptr)
		{
			m_instance = new ComponentPool<C>();
		}

		return m_instance;
	}

	template <class C>
	ComponentPool<C>::ComponentPool()
	{
		// check if the requested type derives from RuntimeComponent
		static_assert(std::is_base_of<RuntimeComponent, C>::value, "Component pool can only be used for subclasses of ECS::RuntimeComponent!");

		// initialize the buffer to 1000 components
		m_storage = std::vector<C>(1000);

		// request type id
		m_type = ComponentPoolBase::RequestTypeID(this);
	}

	// TODO: implement component creation (memory/sorting logic involved)
	template<class C>
	C* ComponentPool<C>::NewComponent()
	{
		return nullptr;
	}

	// TODO: implement component deletion (memory/sorting logic involved)
	template<class C>
	void ComponentPool<C>::DeleteComponent(C* removee)
	{

	}
}

