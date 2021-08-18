#include "pch.h"
#include "EntityManager.h"

#include "ComponentManager.h"

ECS::EntityManager::EntityManager()
{
}

ECS::Entity ECS::EntityManager::NewEntity()
{
	// this event won't trigger anything cuz empty entity doesn't fit in any filter
	Archetype a;
	return NewEntity(a);
}

ECS::Entity ECS::EntityManager::NewEntity(const Archetype& signiture)
{
	Entity e = AvailableEntities.GetEntity();

	if (e != INVALID_ENTITY)
	{
		Signitures[e] = signiture;
	}

	return e;
}

void ECS::EntityManager::DestroyEntity(Entity entity)
{
	AvailableEntities.PutEntity(entity);
	
	// ECSMsgHub::SignalEntityDestoryEvent(entity);
}

ECS::Archetype ECS::EntityManager::GetSigniture(Entity entity)
{
	return Archetype();
}

ECS::EntityManager::EntityRepository::EntityRepository()
{
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		Entities[i] = i;
		EntityValidity[i] = 0;
	}

	// make a min heap
	std::make_heap(Entities.begin(), Entities.end(), std::greater<>());
}

ECS::Entity ECS::EntityManager::EntityRepository::GetEntity()
{
	if (Remaining > 0)
	{
		Entity e = Entities[0];

		if (Remaining > 1)
		{
			std::pop_heap(Entities.begin(), Entities.begin() + Remaining, std::greater<>());
		} // if there's only one entity left, no need to reheap

		Remaining--;

		// TODO: flip bit in validity
		int offset = e % 8;
		int index = e >> 3;
		EntityValidity[index] &= !(1 << offset);

		return e;
	}
	else
	{
		return INVALID_ENTITY;
	}


}

void ECS::EntityManager::EntityRepository::PutEntity(Entity entity)
{
	if (Remaining < MAX_ENTITIES)
	{
		Remaining++;
		Entities[Remaining - 1] = entity;

		std::push_heap(Entities.begin(), Entities.begin() + Remaining, std::greater<>());

		// TODO: flip bit in validity
		int offset = entity % 8;
		int index = entity >> 3;

		EntityValidity[index] &= !(1 << offset);
	}
}

bool ECS::EntityManager::EntityRepository::Has(Entity e)
{
	int index = e >> 3;
	int offset = e % 8;

	return EntityValidity[index] & (1 << offset);
}

