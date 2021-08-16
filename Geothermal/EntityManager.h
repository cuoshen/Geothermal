#pragma once

#include <iostream>
#include <vector>
#include "Archtype.h"
#include "RuntimeComponent.h"

// TODO: it seems this whole class can be made static

namespace ECS 
{
	using EntityID = int;

	class Archetype;

	/// <summary>
	/// the world where all entities reside in
	/// create/delete entities, 
	/// hold all entity data (essentially integers),
	/// hold all component data,
	/// query components
	/// </summary>
	class EntityManager
	{
		friend class EntityToken;

	private: /* fields */
		/// <summary>
		/// Currently how many entities are there.
		/// </summary>
		int entityCount = 0;

		/// <summary>
		/// A list representing each entity.
		/// </summary>
		std::vector<Archetype> entityArctypes;

	public: /* methods */
		/// <summary>
		/// create new entity by allocate new components according to archtype
		/// </summary>
		/// <param name="entityType">the archtype for this new entity</param>
		/// <returns>the ID of new entity</returns>
		EntityID NewEntity(Archetype entityType);

		/// <summary>
		/// batch create new entities of the same type
		/// I'm assuming I don't care about what's created when i call them in batches
		/// </summary>
		/// <param name="entityType">type of new entities</param>
		/// <param name="amount">amount of entities to create in this batch</param>
		void NewEntity(Archetype entityType, int amount);

		/// <summary>
		/// remove the given entity (and its components)
		/// </summary>
		/// <param name="removee"></param>
		void RemoveEntity(EntityID removee);

		/// <summary>
		/// get the entity number this component belongs to
		/// </summary>
		/// <param name="removee">entity of this component</param>
		/// <returns></returns>
		EntityID GetEntity(RuntimeComponent* source);		
	};


	/// <summary>
	/// Temporary type that stores information about a certain entity, 
	/// providing some shorthand methods to operate this entity.
	/// The best way to use it is to always treat it as local variable,
	/// don't store it for later use as entity reference may change every frame.
	/// </summary>
	class EntityToken
	{
	private: /* fields */
		/// <summary>
		/// The entity ID this object points to.
		/// </summary>
		EntityID id;

	public: /* methods */
		EntityToken(EntityID id) : id(id) {}
		
		/// <summary>
		/// Set component data for this entity.
		/// If this entity doesn't have the given component, it does nothing.
		/// This is an almost useless proxy to forbid access of user to the actual entity id.
		/// </summary>
		/// <typeparam name="C">a component type that this entity has</typeparam>
		/// <param name="newData">the data to set (it'll be copied to destination</param>
		/// <returns>EntityToken*: a pointer to self</returns>
		template <class C>
		EntityToken* SetComponentData(C newData);
	};
}

