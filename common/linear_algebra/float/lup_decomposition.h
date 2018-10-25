#pragma once

#include "../matrix.h"
#include "../rows/sub_m.h"
#include "../rows/swap.h"
#include "../../base.h"
#include <algorithm>
#include <cmath>
#include <vector>

class DLUPDecomposition
{
protected:
	double eps_build, eps_solve;

protected:
	TDMatrix lu;
	std::vector<unsigned> p;
	double det_sign;

public:
	DLUPDecomposition(double _eps_build = 1e-10, double _eps_solve = 1e-5) : eps_build(_eps_build), eps_solve(_eps_solve), lu(0), det_sign(1.0) {}

	bool Build(const TDMatrix& m)
	{
        assert(m.Rows() == m.Columns());
		unsigned n = m.Rows();
		lu = m;
		p.resize(n);
		std::vector<double> vv(n);
		det_sign = 1.0;
		for (unsigned i = 0; i < n; ++i)
		{
			double max_value = 0.0;
			for (unsigned j = 0; j < n; ++j)
				max_value = std::max(max_value, fabs(lu(i, j)));
			vv[i] = (max_value < eps_build) ? 1.0 : 1.0 / max_value;
		}
		for (unsigned k = 0; k < n; ++k)
		{
			double max_value = 0.0;
			unsigned imax = k;
			for (unsigned i = k; i < n; ++i)
			{
				double temp = vv[i] * fabs(lu(i, k));
				if (temp > max_value)
				{
					max_value = temp;
					imax = i;
				}
			}
			if (imax != k)
			{
				MatrixRowsSwap(lu, k, imax);
				det_sign = -det_sign;
				vv[imax] = vv[k];
			}
			p[k] = imax;
			if (fabs(lu(k, k)) <= eps_build)
			{
				for (unsigned i = k; i < n; ++i)
				{
					if (fabs(lu(i, k)) > eps_build)
						return false;
					lu(i, k) = 0.0;
				}
			}
			else
			{
				double ilukk = 1.0 / lu(k, k);
				for (unsigned i = k + 1; i < n; ++i)
				{
					lu(i, k) *= ilukk;
					MatrixRowsSubM(lu, i, k, lu(i, k), k + 1);
				}
			}
		}
		return true;
	}

	double Det() const
	{
		double det = det_sign;
		for (unsigned i = 0; i < lu.Rows(); ++i)
			det *= lu(i, i);
		return det;
	}

	// Solve Ax = b
	bool Solve(const TDVector& b, TDVector& output_x)
	{
		unsigned n = lu.Rows();
		assert(b.Size() == n);
		output_x = b;
		unsigned ii = 0;
		for (unsigned i = 0; i < n; ++i)
		{
			unsigned ip = p[i];
			double sum = output_x(ip);
			output_x(ip) = output_x(i);
			if (ii != 0)
			{
				for (unsigned j = ii - 1; j < i; ++j)
					sum -= lu(i, j) * output_x(j);
			}
			else if (sum != 0.0)
				ii = i + 1;
			output_x(i) = sum;
		}
		for (unsigned i = n; i--;)
		{
			double sum = output_x(i);
			for (unsigned j = i + 1; j < n; ++j)
				sum -= lu(i, j) * output_x(j);
			if (lu(i, i) == 0.0)
			{
				if (fabs(sum) > eps_solve)
					return false;
				output_x(i) = 0.0;
			}
			else
				output_x(i) = sum / lu(i, i);
		}
		return true;
	}
};
