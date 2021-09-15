#include "pch.h"
#include "System.h"
#include "ComponentPool.h"

using namespace std;
using namespace Geothermal;
using namespace ECS;

vector<shared_ptr<SystemBase>> SystemManager::Systems;
//int SystemManager::Count = 0;


void Geothermal::ECS::SystemManager::Update()
{
	for (int i = 0; i < Systems.size(); i++)
	{
		Systems[i]->Update(EntityLists[i]);
	}
}

void Geothermal::ECS::SystemManager::LateUpdate()
{
	for (int i = 0; i < Systems.size(); i++)
	{
		Systems[i]->LateUpdate(EntityLists[i]);
	}
}

void SystemManager::OnSignitureChange(Entity e, Archetype newSigniture)
{
	// update entity list for every system (locally)
	for (int i = 0; i < this->EntityLists.size(); i++)
	{
		if (newSigniture.Contains(SystemManager::Systems[i]->GetSigniture()))
		{
			// add the entity, and initialize it with this system
			EntityLists[i].insert(e);
			Systems[i]->Initialize(e);
		}
		else
		{
			EntityLists[i].erase(e);
		}

		if (newSigniture.Contains(SystemManager::ManualSystems[i]->GetSigniture()))
		{
			// add the entity, and initialize it with this system
			ManualEntityLists[i].insert(e);
			ManualSystems[i]->Initialize(e);
		}
		else
		{
			ManualEntityLists[i].erase(e);
		}
	}
}

void SystemManager::OnEntityDestroy(Entity e)
{
	for (auto list : EntityLists)
	{
		list.erase(e);
	}

	for (auto list : ManualEntityLists)
	{
		list.erase(e);
	}
}

