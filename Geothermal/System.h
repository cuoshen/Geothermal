#pragma once

#include <vector>
#include <set>

#include "Defs.h"
#include "ECSMsgHub.h"
#include "Archetype.h"

namespace ECS
{
	class SystemBase
	{
	public:
		virtual void Initialize() {}
		virtual void Update() {}
		virtual void LateUpdate() {}

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

	private:
		static int Count;
		static std::vector<std::shared_ptr<SystemBase>> Systems;
	// end: reflection ///////////////////////////////////////////////////////////////////////////

	public:
		SystemManager();

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
		//Archetype GetSigniture() override
		//{
		//	return Archetype
		//}
	};
}
