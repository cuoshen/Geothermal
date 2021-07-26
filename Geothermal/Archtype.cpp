#include "pch.h"

#include <math.h>
#include "Archtype.h"
#include "ComponentPoolBase.h"
#include "Primes.h"

namespace ECS 
{
	Archetype* Archetype::Add(ComponentPoolBase* newTypePool)
	{
		// get the ID of incoming type
		const int newTypeID = newTypePool->GetType();

		// include the ID if not already in there
		typeCombination = (typeCombination % newTypeID == 0) ? typeCombination * newTypeID : typeCombination;

		// update the biggest type
		if (biggestType < newTypeID)
		{
			biggestType = newTypeID;
		}

		return this;
	}

	bool Archetype::Overlaps(const Archetype& other) const
	{
		// decompose the 2 testers' type number
		const int myTypes = typeCombination;
		const int othersTypes = other.typeCombination;

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

