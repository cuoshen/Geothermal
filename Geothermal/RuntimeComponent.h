#pragma once

namespace ECS {
	/// <summary>
	/// The base class for all runtime conponents.
	/// It also provides a simple integer-based type management, implementing an encapsulated component type management system.
	/// When creating a new component type, always include the following parts:
	/// 1. have a static member initialized to the return value from RequestType();
	/// 2. implement GetType to return that integer.
	/// There's no guard against misuse here.
	/// </summary>
	class RuntimeComponent
	{
	public:
		/// <summary>
		/// Request a unique integer representing a type of components.
		/// </summary>
		/// <returns>integer represnets the calling type</returns>
		static int RequestType();

		/// <summary>
		/// Get the integer representing this derived component type.
		/// </summary>
		/// <returns>type id of the component</returns>
		virtual int GetType() = 0;
	private: 
		/// <summary>
		/// how many types we currently have
		/// </summary>
		static int maxTypes;
	};

	int RuntimeComponent::maxTypes = 0;
}

