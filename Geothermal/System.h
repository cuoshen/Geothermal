#pragma once

#include <vector>
#include <set>

#include "Defs.h"
//#include "Geothermal::ECSMsgHub.h"
#include "Archetype.h"

// TODO: yo, make the system function STATIC HERE!

namespace Geothermal::ECS
{
	class SystemBase
	{
	public:
		virtual void Initialize(std::set<Entity> targets) {}
		virtual void Update(std::set<Entity> targets) {}
		virtual void LateUpdate(std::set<Entity> targets) {}

		virtual Archetype GetSigniture() = 0;
	};

	class SystemManager
	{
	// reflection ////////////////////////////////////////////////////////////////////////////////
	public:
		template <class T>
		static int Register()
		{
			Systems.push_back(std::static_pointer_cast<SystemBase>(std::make_shared<T>()));

			return Count++;
		}

		/// <summary>
		/// Run a specific system (consider it not registered in the reflection).
		/// </summary>
		/// <typeparam name="T"></typeparam>
		template <class T>
		void RunSingle()
		{

		}

		void Initialize();

		void Update();

		void LateUpdate();

		void OnSignitureChange(Entity e, Archetype newSigniture);

		void OnEntityDestroy(Entity e);

	private:
		static int Count;
		static std::vector<std::shared_ptr<SystemBase>> Systems;
	// end: reflection ///////////////////////////////////////////////////////////////////////////

	private:
		std::vector<std::set<Entity>> EntityLists = std::vector<std::set<Entity>>(Count);
	};

	template <class T>
	class ISystem : SystemBase
	{
	private:
		static const int ID;
	};

	template <class T>
	const int ISystem<T>::ID = SystemManager::Register<T>();

	class SampleSystem : public ISystem<SampleSystem>
	{
		//Archetype GetSigniture() override
		//{
		//	return Archetype
		//}
	};
}
