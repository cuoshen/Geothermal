#pragma once

#include <vector>
#include <set>

#include "Defs.h"
#include "ECSMsgHub.h"

namespace ECS
{
	class SystemBase
	{
	public:
		SystemBase();

	public:
		virtual void Initialize() {}
		virtual void Update() {}
		virtual void LateUpdate() {}
	};



	class SystemManager
	{
	// reflection ////////////////////////////////////////////////////////////////////////////////
	public:
		template <class T>
		static int Regsiter()
		{
			Systems.push_back(std::static_pointer_cast<SystemBase>(std::make_shared<T>()));

			return Count++;
		}

	private:
		static int Count;
		static std::vector<std::shared_ptr<SystemBase>> Systems;
	// end: reflection ///////////////////////////////////////////////////////////////////////////

	public:
		SystemManager()
		{
			EntityLists = std::vector<std::set<Entity>>(Count);
			
			ECSMsgHub::RegsiterEntityModifyCallback([this](Entity entity, Archetype newSigniture)
				{
					// TODO: update entity list for every system
				});
		}

	private:
		std::vector<std::set<Entity>> EntityLists;
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
		
	};
}
