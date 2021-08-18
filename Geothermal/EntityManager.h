#pragma once

#include "Defs.h"
#include "Archetype.h"

namespace ECS
{
	struct Archetype;

	class EntityManager
	{
	public:
		EntityManager();

		Entity NewEntity();
		Entity NewEntity(const Archetype& signiture);
		
		void DestroyEntity(Entity entity);
		Archetype GetSigniture(Entity entity);

		// interface to runtime
		template <class T>
		void AddComponentSigniture(Entity e)
		{
			// make sure entity exists
			if (AvailableEntities.Has(e))
			{
				Signitures[e].Add<T>();
			}
		}

		template <class T>
		void RemoveComponentSigniture(Entity e)
		{
			if (AvailableEntities.Has(e))
			{
				Signitures[e].Remove<T>();
			}
		}

	private:
		struct EntityRepository
		{
		public:
			EntityRepository();

			Entity GetEntity();
			void PutEntity(Entity entity);

			// tells if an entity has already been dispatched
			bool Has(Entity e);

		private:
			std::array<Entity, MAX_ENTITIES> Entities;
			int Remaining = MAX_ENTITIES;

			// a bit map to store entities' validity (0 = invalid)
			std::array<char, MAX_ENTITIES / 8> EntityValidity;
		};

		int Count = 0;
		EntityRepository AvailableEntities;
		std::array<Archetype, MAX_ENTITIES> Signitures;
	};
}


