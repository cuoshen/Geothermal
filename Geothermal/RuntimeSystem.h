#pragma once
#include "Archtype.h"

namespace ECS {
	/// <summary>
	/// Systems should not contain any data, only methods
	/// This base class is abstract
	/// System is job (don't separate them into 2 concepts)
	/// Destructor is not defined as systems are not supposed to have data member.
	///
	/// Implement these methods at the very least: Update(), ReadWriteRequest()
	/// </summary>
	class RuntimeSystem
	{
	public: /* static fields */
		static std::vector<RuntimeSystem*> concreteSystems;

	public: /* static methods */
		static void RegisterSystem(RuntimeSystem* newSystem) { concreteSystems.push_back(newSystem); }
		
	public: /* fields */
		/// <summary>
		/// If this system should be executed. Default true.
		/// </summary>
		bool activated = true;

	public: /* methods */
		/// <summary>
		/// When inheriting, make sure to include the base class as it's part of a observer interface.
		/// </summary>
		RuntimeSystem() : activated(true) { RegisterSystem(this); }

		/// <summary>
		/// Handle system tick, should not be overriden.
		/// </summary>
		void PublicTickHandle();

		/// <summary>
		/// Handle system late tick, should not be overriden.
		/// </summary>
		void PublicLateTickHandle();
	
	protected:
		/// <summary>
		/// Override the update function to add custom logic.
		/// </summary>
		virtual void Update() = 0;

		/// <summary>
		/// Override late update function to add logic that's executed after every update function.
		/// </summary>
		virtual void LateUpdate() {}

		/// <summary>
		/// Get the types this system will use.
		/// </summary>
		/// <returns>requested archtype</returns>
		virtual ECS::Archetype ReadWriteRequest() = 0;

		/// <summary>
		/// Get the types this system will read, by default it's empty.
		/// </summary>
		/// <returns>requested archtype</returns>
		virtual Archetype ReadOnlyRequest() { return Archetype(); }

		/// <summary>
		/// Get the types this system reads/writes during late update.
		/// </summary>
		/// <returns></returns>
		virtual Archetype LateReadWriteRequest() { return Archetype(); }

		/// <summary>
		/// Get the types this system reads during late update
		/// </summary>
		/// <returns>requested archtype</returns>
		virtual Archetype LateReadOnlyRequest() { return Archetype(); }
	};
}

