#pragma once
#include <stdint.h>
#include <vector>

namespace Geothermal
{
	using u32 = uint32_t;
	// int can have its name
	using u64 = uint64_t;
	using i64 = int64_t;
	// float can have its name
	// double can have its name

	template <typename T>
	using Vector = std::vector<T>;
}
