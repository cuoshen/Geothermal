#include "pch.h"
#include "ComponentPool.h"

int Geothermal::ECS::IComponentPool::TypeCount = 0;

int Geothermal::ECS::IComponentPool::GetTypeCount()
{
    return TypeCount;
}

int Geothermal::ECS::IComponentPool::Register()
{
    TypeCount++;
    return TypeCount - 1;
}
