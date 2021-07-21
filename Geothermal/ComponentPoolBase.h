#pragma once

#include <unordered_map>
#include <exception>
#include "EntityManager.h"

namespace ECS {
	/// <summary>
	/// The base class of Component pools,
	/// used for type id management.
	/// What does it mean by the error message?
	/// ABSTRACT
	/// </summary>
	class ComponentPoolBase
	{
	private: /* static fields */
		/// <summary>
		/// How many types are there currently.
		/// </summary>
		static int m_TypesCount;

		/// <summary>
		/// map: ID -> Type
		/// </summary>
		static std::unordered_map<int, ComponentPoolBase*> m_IdToPool;

		/// <summary>
		/// map: Type -> ID
		/// </summary>
		static std::unordered_map<ComponentPoolBase*, int> m_PoolToId;

		/// <summary>
		/// The maximum number of types that can be requested.
		/// </summary>
		static int MAX_TYPES;

	private: /* static methods */
		/// <summary>
		/// Given a type number, return a pool instance.
		/// Only open to EntityManager.
		/// </summary>
		/// <param name="typeNum"></param>
		/// <returns></returns>
		static ComponentPoolBase* GetPool(int typeNum);

	protected: /* static methods */
		/// <summary>
		/// Request a unique type ID from a derived component pool singleton instance.
		/// </summary>
		/// <param name="newPool">pinter to the type's factory instance</param>
		/// <returns></returns>
		static int RequestTypeID(ComponentPoolBase* newPool);

	public: /* methods */
		/// <summary>
		/// The inherited method to get the type integer.
		/// </summary>
		/// <returns>integer id of component type</returns>
		virtual int GetType() const = 0;

		// make the default destructor virtual
		virtual ~ComponentPoolBase() = default;

	private: /* friends */
		// entity manager is still within the system so it can access the actual numerical representatio of types
		friend class EntityManager;
	};

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
	int GetNthPrime(int n);
}

