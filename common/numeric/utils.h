#pragma once

#include "../base.h"

namespace {

template<class T>
T GCD(T x, T y)
{
	for (; y; )
	{
		T z = x % y;
		x = y;
		y = z;
	}
	return x;
}

uint64_t DigitsSum(uint64_t x, uint64_t base = 10)
{
	uint64_t s = 0;
	for (; x; x /= base)
		s += x % base;
	return s;
}

}  // namespace
