#pragma once

#include <assert.h>

#include "Primes.h"
#include "Defs.h"

// yo, I capitalized the first letter of member variables intentionally
// if not necessary don't change it back

namespace ECS
{
	/// <summary>
	/// WTF is this? It's only used to make the intellisense work. Don't remove or modify.
	/// </summary>
	class SampleComponent
	{
		int a;
		char b;
	};



	/// <summary>
	/// Base class used to index Component pools.
	/// Sort of a minimal version of reflection for a single type.
	/// Together with the TypeNum member of ComponentPool, other classes can quickly index into each type's pool.
	/// </summary>
	class IComponentPool
	{
	public:
		static int GetTypeCount();

	protected:
		static int Register();

	private:
		static int TypeCount;
	};


	// forward
	class ComponentManager;


	/// <summary>
	/// Data structure to store and manage components of a certain type. 
	/// It's also the factory for components, although components are not comforming to a similar interface.
	/// Most of its methods should not be called outside.
	/// </summary>
	/// <typeparam name="T">a POD component type</typeparam>
	template <class T>
	class ComponentPool : public IComponentPool
	{
	public:
		ComponentPool()
		{
			//// register a lambda to the msg hub
			//ECSMsgHub::RegsiterEntityModifyCallback([this](Entity destroyedEntity)
			//	{
			//		this->Remove(destroyedEntity);
			//	});
		}

		T& Insert(T newComponent, Entity owner)
		{
			//assert(Count < MAX_ENTITIES && "ECS runtime error: Too many components of this type.");
			//assert(EntityToIndex.find(owner) == EntityToIndex.end() && "ECS runtime error: entity alread has this component.");

			// NOTE: I'm not checking entity count here, because that's supposed to be done by entity manager!

			if (/*Count >= MAX_ENTITIES || */EntityToIndex.find(owner) != EntityToIndex.end()) 
			{
				// component already found, return its refe
				return Components[EntityToIndex[owner]];
			}

			// insert
			Components[Count] = newComponent; // copied over
			T& outref = Components[Count];

			// update ownership
			EntityToIndex[owner] = Count;
			IndexToEntity[Count] = owner;

			Count++;

			return outref;
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

	public:
		static int TypeNum;

	private:
		std::array<T, MAX_ENTITIES> Components;
		int Count;

		// managed maps of indices for ownership marking
		std::unordered_map<Entity, int> EntityToIndex;
		std::unordered_map<int, Entity> IndexToEntity;
	};

	template <class T>
	int ComponentPool<T>::TypeNum = IComponentPool::Register();

}


