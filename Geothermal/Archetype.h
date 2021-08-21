#pragma once

#include "ComponentPool.h"

namespace Geothermal::ECS
{
	class ComponentManager;

	/// <summary>
	/// Archetype used to represent entity type combinations.
	/// It's intended to be used on stack only.
	/// </summary>
	struct Archetype
	{
	public:
		Archetype() : Signiture(1) {}

		template <class T>
		Archetype& Add()
		{
			int typePrime = Geothermal::ECS::Tools::primes[ComponentPool<T>::TypeNum];

			if (Signiture != 1 && Signiture % typePrime != 0)
			{
				Signiture *= typePrime;
			}

			return *this;
		}

		template <class T>
		Archetype& Remove()
		{
			int typePrime = Geothermal::ECS::Tools::primes[ComponentPool<T>::TypeNum];

			if (Signiture != 1 && Signiture % typePrime == 0)
			{
				Signiture /= typePrime;
			}

			return *this;
		}

		template <class T>
		bool Contains()
		{
			int typePrime = Geothermal::ECS::Tools::primes[ComponentPool<T>::TypeNum];

			return Signiture != 1 && Signiture % typePrime == 0;
		}

		bool Contains(const Archetype& other)
		{
			// empty archetype is not contained by anyone to avoid useless loops
			return other.Signiture != 1 && Signiture % other.Signiture == 0;
		}

	private:
		friend class ComponentManager;

		int Signiture;
	};
}

