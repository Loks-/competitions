#pragma once

#include "../base.h"

namespace {

uint64_t DigitsSum(uint64_t x, uint64_t base = 10)
{
	uint64_t s = 0;
	for (; x; x /= base)
		s += x % base;
	return s;
}

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

template<class T>
T PowU(T x, uint64_t pow)
{
	T ans = 1;
	for (; pow; pow >>= 1)
	{
		if (pow & 1)
			ans *= x;
		x *= x;
	}
	return ans;	
}

}  // namespace
