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
}

