#include "pch.h"
#include "RuntimeSystem.h"

void ECS::RuntimeSystem::PublicTickHandle()
{
	Update();
}

void ECS::RuntimeSystem::PublicLateTickHandle()
{
	LateUpdate();
}
