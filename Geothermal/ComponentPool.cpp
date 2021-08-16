#include "pch.h"
#include "ComponentPool.h"

int ECS::IComponentPool::GetTypeCount()
{
    return TypeCount;
}

int ECS::IComponentPool::Register()
{
    TypeCount++;
    return TypeCount - 1;
}
