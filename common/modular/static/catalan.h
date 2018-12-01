#pragma once

#include "factorial.h"

template <class TModular>
class ModularCatalan
{
protected:
	ModularFactorial<TModular, true> f;

public:
    TModular Get(unsigned n) { return f.Get(2*n) * f.GetI(n + 1) * f.GetI(n); }
	TModular operator()(unsigned n) { return Get(n); }
    TModular GetF(unsigned n) { return f(n); }
};
