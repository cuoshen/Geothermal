#include "pch.h"
#include "System.h"
#include "ComponentPool.h"

std::vector<std::shared_ptr<Geothermal::ECS::SystemBase>> Geothermal::ECS::SystemManager::Systems;
int Geothermal::ECS::SystemManager::Count = 0;


void Geothermal::ECS::SystemManager::OnSignitureChange(Entity e, Archetype newSigniture)
{
	// update entity list for every system (locally)
	for (int i = 0; i < this->EntityLists.size(); i++)
	{
		if (newSigniture.Contains(SystemManager::Systems[i]->GetSigniture()))
		{
			EntityLists[i].insert(e);
		}
		else
		{
			EntityLists[i].erase(e);
		}
	}
}

void Geothermal::ECS::SystemManager::OnEntityDestroy(Entity e)
{
	for (auto list : EntityLists)
	{
		list.erase(e);
	}
}

