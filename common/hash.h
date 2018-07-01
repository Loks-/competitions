#pragma once

#include "base.h"

namespace std {

inline size_t hash_combine(size_t hvalue1, size_t hvalue2)
{
	return hvalue1 ^ (hvalue2 + 0x9e3779b9 + (hvalue1 << 6) + (hvalue1 >> 2));
}

template<class T1, class T2>
struct hash<pair<T1, T2>>
{
	size_t operator()(const pair<T1, T2>& value) const
	{
		return hash_combine(hash<T1>{}(value.first), hash<T2>{}(value.second));
	}
};

} // namespace std
