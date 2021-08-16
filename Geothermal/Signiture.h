#pragma once

namespace ECS
{
	/// <summary>
	/// Archetype used to represent entity type combinations.
	/// It's intended to be used on stack only.
	/// </summary>
	struct Archetype
	{
	public:
		Archetype& Add(const Archetype& other)
		{
			Signiture *= other.Signiture;
			return *this;
		}

		void Remove(const Archetype& removed)
		{
			if (Signiture % removed.Signiture == 0) Signiture /= removed.Signiture;
		}

		bool Contains(const Archetype& other)
		{
			return Signiture > other.Signiture && Signiture % other.Signiture == 0;
		}

	private:
		int Signiture;
	};
}
