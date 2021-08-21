#include "pch.h"
#include "System.h"
#include "ComponentPool.h"

using namespace std;
using namespace Geothermal;
using namespace ECS;

vector<shared_ptr<SystemBase>> SystemManager::Systems;
int SystemManager::Count = 0;

void SystemManager::OnSignitureChange(Entity e, Archetype newSigniture)
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

void SystemManager::OnEntityDestroy(Entity e)
{
	for (auto list : EntityLists)
	{
		list.erase(e);
	}
}

