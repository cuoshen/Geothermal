#include "pch.h"
#include "ComponentPoolBase.h"

int ECS::ComponentPoolBase::RequestTypeID(ComponentPoolBase* newPool)
{
	if (m_TypesCount >= MAX_TYPES)
	{
		throw TooManyComponentTypesException();
	}

	if (m_PoolToId.count(newPool) >= 1) {
		return m_PoolToId[newPool];
	}
	else {
		m_IdToPool.insert({ m_TypesCount, newPool });
		return m_TypesCount++;
	}
}

