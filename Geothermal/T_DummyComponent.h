#pragma once

#include "RuntimeComponent.h"

namespace ECS_Debug {
	class T_DummyComponent : public ECS::RuntimeComponent
	{
	public:
		int GetType() override { return myType; }
	private:
		static int myType;
	};

	int T_DummyComponent::myType = ECS::RuntimeComponent::RequestType();
}

