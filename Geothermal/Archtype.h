#pragma once
#include <exception>
#include <vector>
#include "ComponentPoolBase.h"

namespace ECS 
{
	class ComponentPoolBase;
	
	/// <summary>
	/// we need a way to combine a vector of component types into uniquely identified archtypes
	/// for the desired usage, we would also value fast creation and deletion of these
	/// </summary>
	class Archetype
	{
	private: /* fields */
		int m_TypeCombination = 1;
		int m_BiggestType = -1;
	
	public: /* methods */		
		/// <summary>
		/// The js-like handler to add a new type into archtype.
		/// The intended usage is like:
		///		Archetype newArchType(); newArchType.AddType(A)->AddType(B);
		///	Caution: only unique types can be added!
		/// </summary>
		/// <param name="newTypePool">pass pointer to a concrete component pool</param>
		/// <returns></returns>
		Archetype* Add(ComponentPoolBase* newTypePool);

		/// <summary>
		/// Returns if this archtype has all the types in the other archtype.
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		bool Contains(const Archetype& other) const { return m_TypeCombination % other.m_TypeCombination == 0; };

		/// <summary>
		/// Similar to Contains(), tests if this is contained by the other archtype completely.
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		bool IsContainedBy(const Archetype& other) const { return other.Contains(*this); }

		/// <summary>
		/// Decides if the 2 giving archtypes have any types in common.
		/// </summary>
		/// <param name="other">the other archtype to test with</param>
		/// <returns>bool: if there's at least one overlapping type</returns>
		bool Overlaps(const Archetype& other) const;
	};
}

