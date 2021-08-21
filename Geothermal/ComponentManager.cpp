#include "pch.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "ECSRuntime.h"
#include "Primes.h"


void Geothermal::ECS::ComponentManager::BatchCreate(Entity e, const Archetype& signiture)
{
	int s = signiture.Signiture;

	// creat components
	for (int i = 0; i < IComponentPool::GetTypeCount() && s > 1; i++)
	{
		if (s % Geothermal::ECS::Tools::primes[i] == 0)
		{
			s /= Geothermal::ECS::Tools::primes[i];
			DynamicCreationCallbacks[i](e);
		}
	}
}

void Geothermal::ECS::ComponentManager::OnEntityDestroy(Entity e)
{
	for (auto callback : OnEntityDestroyCallbacks)
	{
		if (callback)
			callback(e);
	}
}

