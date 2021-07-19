#include "pch.h"
#include "ComponentPoolBase.h"
#include "Primes.h"

// TODO: implement the function
int ECS::GetNthPrime(int n)
{
	return ECS_Tools::primes[n];
}

// TODO: give each request a prime number instead of consecutive integers.
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
		m_IdToPool.insert({ GetNthPrime(m_TypesCount), newPool });
		m_TypesCount++;
		return GetNthPrime(m_TypesCount);
	}
}

