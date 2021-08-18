#include "pch.h"
#include "ECSRuntime.h"

#include "System.h"
#include "Primes.h"


// no need to signal anything
ECS::Entity ECS::Runtime::NewEntity()
{
	return entityManager.NewEntity();
}

ECS::Entity ECS::Runtime::NewEntity(const Archetype& signiture)
{
	Entity newEntity = entityManager.NewEntity(signiture);
	
	if (newEntity != INVALID_ENTITY)
	{
		componentManager.BatchCreate(newEntity, signiture);

		// refresh systems
		systemManager.OnSignitureChange(newEntity, signiture);
	}

	return newEntity;
}

void ECS::Runtime::DestroyEntity(Entity e)
{
	entityManager.DestroyEntity(e);
	componentManager.OnEntityDestroy(e);
	systemManager.OnEntityDestroy(e);
}

