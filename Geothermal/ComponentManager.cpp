#include "pch.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "ECSRuntime.h"

ECS::ComponentManager::ComponentManager(Runtime* runtime) : runtime(runtime)
{
}

void ECS::ComponentManager::OnEntityDestroy(Entity e)
{
	for (auto callback : OnEntityDestroyCallbacks)
	{
		callback(e);
	}
}

void ECS::ComponentManager::EntitySignalProxy(Entity e)
{

}

