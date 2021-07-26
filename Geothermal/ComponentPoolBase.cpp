#include "pch.h"
#include "ComponentPoolBase.h"
#include "Primes.h"

using namespace ECS;

/* initialize static members */
int ComponentPoolBase::m_TypesCount = 0;
std::unordered_map<int, ComponentPoolBase*> ComponentPoolBase::m_IdToPool = std::unordered_map<int, ComponentPoolBase*>();
std::unordered_map<ComponentPoolBase*, int> ComponentPoolBase::m_PoolToId = std::unordered_map<ComponentPoolBase*, int>();
int ComponentPoolBase::MAX_TYPES = 1000;

// I removed this since there's no chance a user uses more than 100 components
///// <summary>
///// Custom exception class to handle type creation.
///// </summary>
//class TooManyComponentTypesException : public std::exception
//{
//	const char * what () {
//		return "ECS Error: number of component types exceeds max defined in ComponentPoolBase.h";
//	}
//};

/// <summary>
/// Utility function, find the nth prime number, start at 0.
/// </summary>
/// <param name="n">the index of prime</param>
/// <returns>the nth prime number</returns>
int GetNthPrime(int n)
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

