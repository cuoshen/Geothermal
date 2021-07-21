#include "pch.h"
#include "ComponentPoolBase.h"
#include "Primes.h"

int ECS::GetNthPrime(int n)
{
	return ECS_Tools::primes[n];
}

ECS::ComponentPoolBase* ECS::ComponentPoolBase::GetPool(int typeNum)
{
	if (m_IdToPool.count(typeNum) >= 1)
	{
		return m_IdToPool[typeNum];
	}
	else
	{
		return nullptr;
	}
}

// give each request a prime number instead of consecutive integers.
int ECS::ComponentPoolBase::RequestTypeID(ComponentPoolBase* newPool)
{
	if (m_PoolToId.count(newPool) >= 1) 
	{
		return m_PoolToId[newPool];
	}
	else 
	{
		m_IdToPool.insert({ GetNthPrime(m_TypesCount), newPool });
		m_PoolToId.insert({ newPool, GetNthPrime(m_TypesCount) });
		m_TypesCount++;
		return GetNthPrime(m_TypesCount);
	}
}

