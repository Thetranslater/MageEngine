#pragma once

#include<cstddef>
#include<functional>

namespace Mage {
	template<typename T>
	inline void hash_combine(size_t& value, const T& gen) {
		value ^= std::hash<T>{}(gen)+(value << 6) + (value >> 2);
	}

	template<typename T, typename... Ts>
	inline void hash_combine(size_t& value, const T& gen, const Ts&... gens) {
		value ^= std::hash<T>{}(gen)+(value << 6) + (value >> 2);
		hash_combine(value, gens...);
	}
}