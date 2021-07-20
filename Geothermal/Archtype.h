#pragma once
#include <exception>
#include "ComponentPoolBase.h"
#include <vector>

namespace ECS {
	/// <summary>
	/// we need a way to combine a vector of component types into uniquely identified archtypes
	/// </summary>
	class Archtype
	{
	private: /* fields */
		int m_TypeCombination;
	
	public:
		/// <summary>
		/// The default constructor that does nothing.
		/// </summary>
		Archtype() : m_TypeCombination(1) {}
		
		/// <summary>
		/// The js-like handler to add a new type into archtype.
		/// The intended usage is like:
		///		Archtype newArchType = Archtype()->AddType(A)->AddType(B);
		///	Caution: only unique types can be added!
		/// </summary>
		/// <param name="newTypePool">pass pointer to a concrete component pool</param>
		/// <returns></returns>
		Archtype* Add(ComponentPoolBase* newTypePool);

		/// <summary>
		/// Returns if this archtype has all the types in the other archtype.
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		bool Contains(const Archtype& other) const { return m_TypeCombination % other.m_TypeCombination == 0; };

		/// <summary>
		/// Similar to Contains(), tests if this is contained by the other archtype completely.
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		bool IsContainedBy(const Archtype& other) const { return other.Contains(*this); }
	};
}

