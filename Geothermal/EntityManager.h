#pragma once

#include <iostream>
#include <vector>
#include "Archtype.h"
#include "RuntimeComponent.h"


namespace ECS {
	using EntityID = int;

	/// <summary>
	/// the world where all entities reside in
	/// create/delete entities, 
	/// hold all entity data (essentially integers),
	/// hold all component data,
	/// query components
	/// </summary>
	class EntityManager
	{
	public:
		/// <summary>
		/// create new entity by allocate new components according to archtype
		/// </summary>
		/// <param name="entityType">the archtype for this new entity</param>
		/// <returns>the ID of new entity</returns>
		EntityID NewEntity(ECS::Archtype entityType);

		/// <summary>
		/// batch create new entities of the same type
		/// I'm assuming I don't care about what's created when i call them in batches
		/// </summary>
		/// <param name="entityType">type of new entities</param>
		/// <param name="amount">amount of entities to create in this batch</param>
		void NewEntity(ECS::Archtype entityType, int amount);

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

}

