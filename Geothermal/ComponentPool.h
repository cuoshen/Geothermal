#pragma once

#include "Defs.h"
#include "ECSMsgHub.h"
#include <assert.h>

// yo, I capitalized the first letter of member variables intentionally
// if not necessary don't change it back

namespace ECS
{
	class SampleComponent
	{
		int a;
		char b;
	};

	//class ComponentPoolBase
	//{
	//public:
	//	/// <summary>
	//	/// Event callback for 
	//	/// </summary>
	//	/// <param name="destroyedEntity"></param>
	//	virtual void OnEntityDestroyed(Entity destroyedEntity) = 0;
	//};

	// TODO: implement its singleton functionalities

	/// <summary>
	/// Data structure to store and manage components of a certain type. 
	/// Most of its methods should not be called outside.
	/// </summary>
	/// <typeparam name="T">a POD component type</typeparam>
	template <class T>
	class ComponentPool /*: public ComponentPoolBase*/
	{
	public:
		ComponentPool()
		{
			// register a lambda to the msg hub
			ECSMsgHub::RegsiterEntityDestoryCallback([this](Entity destroyedEntity) { 
				this->Remove(destroyedEntity);
			});
		}

		void Insert(T newComponent, Entity owner)
		{
			assert(Count < MAX_ENTITIES && "ECS runtime error: Too many components of this type.");
			assert(EntityToIndex.find(owner) == EntityToIndex.end() && "ECS runtime error: entity alread has this component.");

			// insert
			Components[Count] = newComponent; // copied over

			// update ownership
			EntityToIndex[owner] = Count;
			IndexToEntity[Count] = owner;

			Count++;
		}
		
		void Remove(Entity owner)
		{
			assert(EntityToIndex.find(owner) != EntityToIndex.end() && "ECS runtime error: Target owner doesn't exist.");

			// cache some indices
			int targetIndex = EntityToIndex[owner];
			Entity replacerEntity = IndexToEntity[Count - 1];

			// replace and remove
			Components[targetIndex] = Components[Count - 1];
			EntityToIndex.erase(owner);
			IndexToEntity.erase(Count - 1);

			// reconnect ownership mapping
			EntityToIndex[replacerEntity] = targetIndex;
			IndexToEntity[targetIndex] = replacerEntity;

			Count--;
		}

		T& Get(Entity owner)
		{
			assert(EntityToIndex.find(owner) != EntityToIndex.end() && "ECS runtime error: Can't find requested owner.");

			return Components[EntityToIndex[owner]];
		}

	private:
		std::array<T, MAX_ENTITIES> Components;
		int Count;

		// managed maps of indices for ownership marking
		std::unordered_map<Entity, int> EntityToIndex;
		std::unordered_map<int, Entity> IndexToEntity;
	};
}


