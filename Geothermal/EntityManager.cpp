#include "pch.h"
#include "EntityManager.h"

ECS::EntityID ECS::EntityManager::NewEntity(ECS::Archetype entityType)
{
	// enxtend entity id with entity count
	EntityID newEntity = m_EntityCount++;

	// TODO: extract all types in the archetype
	

	// TODO: request new components and label them with the new entity


	// TODO: add archetype to list
	
	return EntityID();
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

