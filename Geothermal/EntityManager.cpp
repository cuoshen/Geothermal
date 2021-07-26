#include "pch.h"
#include "EntityManager.h"
#include "ComponentPool.h"

// EntityManager ////////////////


// EntityToken //////////////////
template<class C>
ECS::EntityToken* ECS::EntityToken::SetComponentData(C newData)
{
	// attempt to set data
	if (ECS::ComponentPool<C>::GetInstance()->SetComponentData(m_ID, newData))
	{
		// TODO: if you need to do anything, add it here
	}
	else
	{
		// TODO: same as above
	}
	
	return nullptr;
}