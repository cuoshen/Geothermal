#pragma once

#include <vector>
#include <set>

#include "Defs.h"
//#include "Geothermal::ECSMsgHub.h"
#include "Archetype.h"

// TODO: yo, make the system function STATIC HERE!
// TODO: the problem persists, that I can't store a bunch of type parameters in a list and run them.
// good thing is, manual invocation is seemingly in place

namespace Geothermal::ECS
{
	class SystemBase
	{
	public:
		/// <summary>
		/// Override to perform startup operation on a newly added entity (equivalent to Start() in Unity)
		/// If multiple systems capture the same entity, there's no guarantee on the order of execution.
		/// </summary>
		/// <param name="target">the entity just added here</param>
		virtual void Initialize(Entity target) {}

		/// <summary>
		/// Update all captured entities every frame.
		/// </summary>
		/// <param name="targets">entities captured</param>
		virtual void Update(std::set<Entity> targets) {}

		/// <summary>
		/// A second half of the frame, updates after Update.
		/// </summary>
		/// <param name="targets">entities captured</param>
		virtual void LateUpdate(std::set<Entity> targets) {}

		/// <summary>
		/// Return a signiture of what this system updates on.
		/// </summary>
		/// <returns></returns>
		virtual Archetype GetSigniture() = 0;
	};


	/// <summary>
	/// Base class for a system that you want the framework to update for you.
	/// This includes entity capture list changes whenever entities have a change,
	/// and the update / lateupdate events on every single frame.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <class T>
	class ISystem : SystemBase
	{
	private:
		static const int ID;
	};


	/// <summary>
	/// Base class for a system that you want to update yourself.
	/// The framework will only update its capture list, nothing else.
	/// It's designed for internal usage, like render system, but since this ECS is also designed to use locally, it should come in handy somewhere else as well.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <class T>
	class IManualSystem : SystemBase
	{
	public:
		static int GetID() { return ID; }
	private:
		static const int ID;
	};


	class SystemManager
	{
	// auto invocation ///////////////////////////////////////////////////////////////////////////
	public:
		template <class T>
		static int Register()
		{
			Systems.push_back(std::static_pointer_cast<SystemBase>(std::make_shared<T>()));

			return Systems.size() - 1;
		}

	private:
		//static int Count;
		static std::vector<std::shared_ptr<SystemBase>> Systems;

	private:
		std::vector<std::set<Entity>> EntityLists = std::vector<std::set<Entity>>(Systems.size());
	// end: auto invocation //////////////////////////////////////////////////////////////////////


	// manual invocation /////////////////////////////////////////////////////////////////////////
	// basically the same thing as the previous part, but it's not automatically updated
	public:
		/// <summary>
		/// This means: run the list updates, not frame udpates
		/// </summary>
		/// <typeparam name="T"></typeparam>
		template <class T>
		static int ManualRegister()
		{
			ManualSystems.push_back(std::static_pointer_cast<SystemBase>(std::make_shared<T>()));
			return ManualSystems.size() - 1;
		}

	private:
		static std::vector<std::shared_ptr<SystemBase>> ManualSystems;

	public:
		template <class T>
		void ManualUpdate()
		{
			ManualSystems[IManualSystem<T>::GetID()]->Update(ManualEntityLists[IManualSystem<T>::GetID()]);
		}

		template <class T>
		void ManualLateUpdate()
		{
			ManualSystems[IManualSystem<T>::GetID()]->LateUpdate(ManualEntityLists[IManualSystem<T>::GetID()]);
		}

	private:
		std::vector<std::set<Entity>> ManualEntityLists = std::vector<std::set<Entity>>(Systems.size());
	// end: manual invocation ////////////////////////////////////////////////////////////////////

	public:
		void Update();
		void LateUpdate();

		void OnSignitureChange(Entity e, Archetype newSigniture);
		void OnEntityDestroy(Entity e);
	};


	template <class T>
	const int ISystem<T>::ID = SystemManager::Register<T>();

	template <class T>
	const int IManualSystem<T>::ID = SystemManager::ManualRegister<T>();


	class SampleSystem : public ISystem<SampleSystem>
	{
		Archetype GetSigniture() override
		{
			return Archetype();
		}

		virtual void Update(std::set<Entity> targets) override
		{

		}
	};
}
