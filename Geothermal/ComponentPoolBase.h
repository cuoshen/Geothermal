#pragma once

#include <unordered_map>

namespace ECS {
	/// <summary>
	/// The base class of Component pools,
	/// used for type id management.
	/// </summary>
	class ComponentPoolBase
	{
	public:
		static int RequestTypeID(ComponentPoolBase* newPool);

	private:
		static int m_MaxTypes;
		static std::unordered_map<int, ComponentPoolBase*> m_IdToPool;
		static std::unordered_map<ComponentPoolBase*, int> m_PoolToId;
	};

	int ComponentPoolBase::m_MaxTypes = 0;
	std::unordered_map<int, ComponentPoolBase*> ComponentPoolBase::m_IdToPool = std::unordered_map<int, ComponentPoolBase*>();
	std::unordered_map<ComponentPoolBase*, int> ComponentPoolBase::m_PoolToId = std::unordered_map<ComponentPoolBase*, int>();
}

