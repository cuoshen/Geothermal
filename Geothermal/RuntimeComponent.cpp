#include "pch.h"
#include "RuntimeComponent.h"

namespace ECS {
	int RuntimeComponent::RequestType() 
	{
		return maxTypes++;
	}
}

