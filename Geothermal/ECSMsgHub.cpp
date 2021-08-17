#include "pch.h"
#include "ECSMsgHub.h"

std::vector<std::function<void(ECS::Entity)>> ECS::ECSMsgHub::EntityDestroyCallbacks;
std::vector<std::function<void(ECS::Entity, ECS::Archetype)>> ECS::ECSMsgHub::EntityModifyCallbacks;

void ECS::ECSMsgHub::SignalEntityDestoryEvent(Entity entity)
{
	for (auto callBack : EntityDestroyCallbacks)
	{
		callBack(entity);
	}
}

void ECS::ECSMsgHub::RegsiterEntityDestoryCallback(std::function<void(Entity)> newCallback)
{
	EntityDestroyCallbacks.push_back(newCallback);
}

void ECS::ECSMsgHub::SignalEntityModifyEvent(Entity entity, Archetype newSigniture)
{
	for (auto callback : EntityModifyCallbacks)
	{
		callback(entity, newSigniture);
	}
}

void ECS::ECSMsgHub::RegsiterEntityModifyCallback(std::function<void(Entity, Archetype)> newCallback)
{
	EntityModifyCallbacks.push_back(newCallback);
}

