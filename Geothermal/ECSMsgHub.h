#pragma once

#include "Defs.h"
#include <vector>
#include <functional>

namespace ECS
{
	class ECSMsgHub
	{
	public:
		static void RegsiterEntityDestoryCallback(std::function<void(Entity)> newCallback)
		{
			EntityDestroyCallbacks.push_back(newCallback);
		}

	private:
		static std::vector<std::function<void(Entity)>> EntityDestroyCallbacks;
	};
}

