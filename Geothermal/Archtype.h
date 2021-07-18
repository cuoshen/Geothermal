#pragma once
#include <exception>

namespace ECS {
	/// <summary>
	/// we need a way to combine a vector of component types into uniquely identified archtypes
	/// </summary>
	class Archtype
	{
	public:
		/// <summary>
		/// request a type identifier for your component
		/// </summary>
		/// <returns></returns>
		static int RequestTypeNumber();

		void SetTypes(int types, ...);
	};

	struct ArchtypeParameterException : public std::exception {
		const char* what() const throw () {
			return "Archtype parameter exception: Wrong argument to archtype constructor!";
		}
	};
}

