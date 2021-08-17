#include "pch.h"
#include "System.h"
#include "ComponentPool.h"

std::vector<std::shared_ptr<ECS::SystemBase>> ECS::SystemManager::Systems;
int ECS::SystemManager::Count = 0;


ECS::SystemManager::SystemManager()
{
	EntityLists = std::vector<std::set<Entity>>(Count);

	ECSMsgHub::RegsiterEntityModifyCallback([this](Entity entity, Archetype newSigniture)
		{
			// update entity list for every system (locally)
			for (int i = 0; i < this->EntityLists.size(); i++)
			{
				if (newSigniture.Contains(SystemManager::Systems[i]->GetSigniture()))
				{
					this->EntityLists[i].insert(entity);
				}
				else
				{
					this->EntityLists[i].erase(entity);
				}
			}
		});
}
