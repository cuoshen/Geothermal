#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

namespace ECS {
	class RuntimeComponentBase
	{
		// type management
	private:
		/// <summary>
		/// return the ID of this new type and increment type count
		/// </summary>
		/// <returns></returns>
		static int AddNewType();
		static int maxTypes;

	public:
		static int RegisterType();
	};
}

