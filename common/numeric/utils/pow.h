#pragma once

#include "../../base.h"

template<class T>
inline T PowU(T x, uint64_t pow)
{
	T ans = T(1);
	for (; pow; pow >>= 1)
	{
		if (pow & 1)
			ans *= x;
		x *= x;
	}
	return ans;	
}
