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
	public: /* fields */
		/// <summary>
		/// Since we are using vector storage, we'll need a way to tell if a buffer member is used or not.
		/// </summary>
		bool isUsed = false;
	};
}

