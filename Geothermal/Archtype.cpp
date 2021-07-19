#include "pch.h"
#include "Archtype.h"

namespace ECS {
	Archtype* Archtype::Add(ComponentPoolBase* newTypePool)
	{
		// get the ID of incoming type
		const int newTypeID = newTypePool->GetType();

		// include the ID if not already in there
		m_TypeCombination = (m_TypeCombination % newTypeID == 0) ? m_TypeCombination * newTypeID : m_TypeCombination;

		return this;
	}
}

