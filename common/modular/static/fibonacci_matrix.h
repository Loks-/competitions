#pragma once

#include "../../base.h"
#include "../../linear_algebra/matrix_static_size.h"

template<class TModular>
class ModularFibonacciMatrix
{
public:
	using TMatrix = TSquareMatrixStaticSize<TModular, 2>;

	static TModular GetU(uint64_t n, TModular f0, TModular f1)
	{
		TMatrix transition;
		transition(0, 0) = 0;
		transition(0, 1) = 1;
		transition(1, 0) = 1;
		transition(1, 1) = 1;
		TMatrix fn = transition.PowU(n);
		return fn(0, 0) * f0 + fn(0, 1) * f1;
	}
	static TModular GetU(uint64_t n) { return GetU(n, 0, 1); }
};
