#pragma once

#include "../hash.h"
#include <utility>

namespace std {

template<class T1, class T2>
struct hash<std::pair<T1, T2>>
{
	size_t operator()(const std::pair<T1, T2>& value) const
	{
		return hash_combine(hash<T1>{}(value.first), hash<T2>{}(value.second));
	}
};

} // namespace std
