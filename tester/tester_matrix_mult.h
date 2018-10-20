#pragma once

#include "common/hash.h"
#include "common/linear_algebra/matrix.h"
#include "common/modular/modular.h"
#include "common/modular/modular_arithmetic_static_proxy.h"

#include "matrix_mult.h"

template<unsigned matrix_size>
class TesterMatrixMult
{
public:
	using TModular = Modular<>;
	using TModularProxy = ModularArithmeticStaticProxy<>;

public:
	static size_t MatrixHash(const Matrix<TModular>& m)
	{
		size_t h = 0;
		for (TModular t : m.GetData())
			h = hash_combine(h, t.Get());
		return h;
	}

	static size_t MatrixHash(const Matrix<uint64_t>& m)
	{
		size_t h = 0;
		for (uint64_t t : m.GetData())
			h = hash_combine(h, TModularProxy::ApplyU(t));
		return h;
	}

protected:
	Matrix<uint64_t> muA, muB, muC;
	Matrix<TModular> mmA, mmB, mmB;
};
