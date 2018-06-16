#pragma once

#include "../base.h"

// Time and space to initialize to size n is O(n^2).
// For big n and prime module it's better to use FactorialModular class.
template <class TModular>
class BinomialCoefficientModular
{
protected:
	vector<vector<TModular>> nCr_table;

public:
	void Adjust(unsigned n)
	{
		for (; nCr_table.size() <= n; )
		{
			vector<TModular> vnext(nCr_table.size() + 1, TModular(1));
			for (unsigned i = 1; i + 1 < vnext.size(); ++i)
			{
				vnext[i] = nCr_table.back()[i - 1] + nCr_table.back()[i];
			}
			nCr_table.push_back(vnext);
		}
	}

	TModular operator()(unsigned n, unsigned k)
	{ 
		if (k > n) return 0;
		Adjust(n);
		return nCr_table[n][k];
	}
};
