#include "pch.h"
#include "EntityManager.h"

#include "ComponentManager.h"
#include "ECSMsgHub.h"

ECS::EntityManager::EntityManager()
{
}

ECS::Entity ECS::EntityManager::NewEntity()
{
	// this event won't trigger anything cuz empty entity doesn't fit in any filter
	return Entity();
}

ECS::Entity ECS::EntityManager::NewEntity(Archetype& signiture)
{
	Entity e = AvailableEntities.GetEntity();

	if (e != INVALID_ENTITY)
	{
		// TODO: create components for it
	}

	return e;
}

void ECS::EntityManager::DestroyEntity(Entity entity)
{
	AvailableEntities.PutEntity(entity);
	
	ECSMsgHub::SignalEntityDestoryEvent(entity);
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
	}
}

