#pragma once

#include "Defs.h"

namespace ECS
{
	struct Archetype;

	class EntityManager
	{
	public:
		EntityManager();

		Entity NewEntity();
		Entity NewEntity(Archetype& signiture);
		
		void DestroyEntity(Entity entity);
		Archetype GetSigniture(Entity entity);

	private:
		struct EntityRepository
		{
		public:
			EntityRepository();

			Entity GetEntity();
			void PutEntity(Entity entity);
		private:
			std::array<Entity, MAX_ENTITIES> Entities;
			int Remaining = MAX_ENTITIES;
		};

		int Count = 0;
		EntityRepository AvailableEntities;
		std::array<Archetype, MAX_ENTITIES> Signitures;
	};
}


