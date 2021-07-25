#include "pch.h"
#include "T_DummySystem.h"
#include "T_DummyComponent.h"
#include "ComponentTypeHelpers.h"

void ECS_Debug::T_DummySystem::Update()
{
	// does nothing
}

ECS::Archtype ECS_Debug::T_DummySystem::ReadWriteRequest()
{
	ECS::Archtype a;
	a.Add(ECS::TypeOf<ECS_Debug::T_DummyComponent>());
	return a;
}

