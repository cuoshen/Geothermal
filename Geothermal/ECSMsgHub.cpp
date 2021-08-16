#include "pch.h"
#include "ECSMsgHub.h"

std::vector<std::function<void(ECS::Entity)>> ECS::ECSMsgHub::EntityDestroyCallbacks;

void ECS::ECSMsgHub::SignalEntityDestoryEvent(Entity entity)
{
	for (auto callBack : EntityDestroyCallbacks)
	{
		callBack(entity);
	}
}
