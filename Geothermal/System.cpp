#include "pch.h"
#include "System.h"

std::vector<std::shared_ptr<ECS::SystemBase>> ECS::SystemManager::Systems;
int ECS::SystemManager::Count = 0;

ECS::SystemBase::SystemBase()
{

}
