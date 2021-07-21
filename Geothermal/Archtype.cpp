#include <math.h>
#include "pch.h"
#include "Archtype.h"
#include "Primes.h"

namespace ECS 
{
	Archtype* Archtype::Add(ComponentPoolBase* newTypePool)
	{
		// get the ID of incoming type
		const int newTypeID = newTypePool->GetType();

		// include the ID if not already in there
		m_TypeCombination = (m_TypeCombination % newTypeID == 0) ? m_TypeCombination * newTypeID : m_TypeCombination;

		return this;
	}

	bool Archtype::Overlaps(const Archtype& other) const
	{
		// decompose the 2 testers' type number
		const int myTypes = m_TypeCombination;
		const int othersTypes = other.m_TypeCombination;

		// set the termination point
		const int maxPossibleCommonFactor = min(myTypes, othersTypes);

		// loop all possible common factors
		for (int i = 0; i < 1000; i++)
		{
			const int currentPrime = ECS_Tools::primes[i];
			if ((myTypes % currentPrime == 0) && (othersTypes % currentPrime == 0))
			{
				return true;
			}
		}

		// if there's no qualifying prime, deny it
		return false;
	}
}

