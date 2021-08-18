#include "pch.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "ECSRuntime.h"
#include "Primes.h"


void ECS::ComponentManager::BatchCreate(Entity e, const Archetype& signiture)
{
	int s = signiture.Signiture;

	// creat components
	for (int i = 0; i < IComponentPool::GetTypeCount() && s > 1; i++)
	{
		if (s % ECS_Tools::primes[i] == 0)
		{
			s /= ECS_Tools::primes[i];
			DynamicCreationCallbacks[i](e);
		}
	}
}

void ECS::ComponentManager::OnEntityDestroy(Entity e)
{
	for (auto callback : OnEntityDestroyCallbacks)
	{
		if (callback)
			callback(e);
	}
}

