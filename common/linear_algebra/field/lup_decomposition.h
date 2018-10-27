#pragma once

#include "../rows/sub_m.h"
#include "../rows/swap.h"
#include "../../base.h"
#include <vector>

template <class TTMatrix>
class FLUPDecomposition
{
public:
    using TMatrix = TTMatrix;
    using TValue = typename TMatrix::TValue;

protected:
	TMatrix lu;
	std::vector<unsigned> p;
	TValue det_sign;

public:
	FLUPDecomposition() : lu(0), det_sign(1) {}

	bool Build(const TMatrix& m)
	{
        assert(m.Rows() == m.Columns());
		unsigned n = m.Rows();
		lu = m;
		p.resize(n);
		det_sign = 1;
		for (unsigned k = 0; k < n; ++k)
		{
            p[k] = k;
            for (unsigned i = k; i < n; ++i)
            {
                if (lu(i, k) != 0)
                {
                    if (i != k)
                    {
                        MatrixRowsSwap(lu, k, i);
                        det_sign = -det_sign;
            			p[k] = i;
                    }
                    break;
                }
            }
			if (lu(k, k) == 0)
			{
				for (unsigned i = k; i < n; ++i)
				{
					if (lu(i, k) != 0)
						return false;
				}
			}
			else
			{
				TValue ilukk = TValue(1) / lu(k, k);
				for (unsigned i = k + 1; i < n; ++i)
				{
					lu(i, k) *= ilukk;
					MatrixRowsSubM(lu, i, k, lu(i, k), k + 1);
				}
			}
		}
		return true;
	}

	TValue Det() const
	{
		TValue det = det_sign;
		for (unsigned i = 0; i < lu.Rows(); ++i)
			det *= lu(i, i);
		return det;
	}

	// Solve Ax = b
    template<class TVector>
	bool Solve(const TVector& b, TVector& output_x)
	{
		unsigned n = lu.Rows();
		assert(b.Size() == n);
		output_x = b;
		unsigned ii = 0;
		for (unsigned i = 0; i < n; ++i)
		{
			unsigned ip = p[i];
			TValue sum = output_x(ip);
			output_x(ip) = output_x(i);
			if (ii != 0)
			{
				for (unsigned j = ii - 1; j < i; ++j)
					sum -= lu(i, j) * output_x(j);
			}
			else if (sum != 0)
				ii = i + 1;
			output_x(i) = sum;
		}
		for (unsigned i = n; i--;)
		{
			TValue sum = output_x(i);
			for (unsigned j = i + 1; j < n; ++j)
				sum -= lu(i, j) * output_x(j);
			if (lu(i, i) == 0)
			{
				if (sum != 0)
					return false;
				output_x(i) = 0;
			}
			else
				output_x(i) = sum / lu(i, i);
		}
		return true;
	}
};
