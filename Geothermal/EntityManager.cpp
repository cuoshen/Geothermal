#include "pch.h"
#include "EntityManager.h"
#include "ComponentPool.h"
#include "Primes.h"
#include "Archtype.h"

// IMPORTANT: this function basically assumes when adding an entity, it's *always* currently biggest in index.
ECS::EntityID ECS::EntityManager::NewEntity(ECS::Archetype entityType)
{
	// enxtend entity id with entity count
	EntityID newEntity = entityCount++;

	// extract all types in the archetype
	if (entityType.biggestType != -1)
	{
		// there are types
		// get the biggest type
		int biggestType = entityType.biggestType;
		// TODO: add this component

		// process other types
		int remainingTypes = entityType.typeCombination / biggestType;
		for (int i = 0; ECS_Tools::primes[i] < remainingTypes; i++)
		{
			// 
			int currentType = ECS_Tools::primes[i];

			// skip types that's not in here
			if (remainingTypes % currentType != 0) continue;

			// update remaining types
			remainingTypes /= currentType;

			// add this component
			// note: implemented this way will push all the difficult work on the entity removal routine
			entityArctypes.push_back(entityType);
		}
	}
	else
	{
		// this is an error message, input entityType is empty
		return EntityID(-1);
	}

	// TODO: cache archetype

	return newEntity;
}

void ECS::EntityManager::NewEntity(ECS::Archetype entityType, int amount)
{
}

void ECS::EntityManager::RemoveEntity(EntityID removee)
{
}

ECS::EntityID ECS::EntityManager::GetEntity(RuntimeComponent* source)
{
	return EntityID();
}

