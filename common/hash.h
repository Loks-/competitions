#pragma once

#include "base.h"
#include <functional>

inline size_t hash_combine(size_t hvalue1, size_t hvalue2)
{
	return hvalue1 ^ (hvalue2 + 0x9e3779b9 + (hvalue1 << 6) + (hvalue1 >> 2));
}
