#include "pch.h"
#include "ECSRuntime.h"

#include "EntityManager.h"
#include "ComponentManager.h"
#include "System.h"

ECS::Runtime::Runtime()
{
	entityManager = std::make_shared<EntityManager>();
	componentManager = std::make_shared<ComponentManager>();
	systemManager = std::make_shared<SystemManager>();
}

//void ECS::Runtime::RegisterEntityDestroyCallback(std::function<void(Entity)> callback)
//{
//	EntityDestroyCallbacks.push_back(callback);
//}
//
//void ECS::Runtime::RegisterEntityModifyCallback(std::function<void(Entity, Archetype)> callback)
//{
//	EntityModifyCallbacks.push_back(callback);
//}
//
//void ECS::Runtime::SignalEntityDestroyEvent(Entity entity)
//{
//	for (auto callback : EntityDestroyCallbacks)
//	{
//		callback(entity);
//	}
//}
//
//void ECS::Runtime::SignalEntityModifyEvent(Entity entity, Archetype signiture)
//{
//	for (auto callback : EntityModifyCallbacks)
//	{
//		callback(entity, signiture);
//	}
//}
