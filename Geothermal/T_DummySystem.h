#pragma once
#include "EntityComponentSystem.h"

namespace ECS_Debug
{
	class T_DummySystem : public ECS::RuntimeSystem
	{
		void Update() override;
		ECS::Archetype ReadWriteRequest() override;
	};
}
