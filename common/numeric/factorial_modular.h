#pragma once

#include "modular.h"

template <class TModular, bool keep_inverted = true>
class FactorialModular
{
protected:
	vector<TModular> vf, vfi;

public:
	FactorialModular() { vf.push_back(TModular(1)); if (keep_inverted) vfi.push_back(TModular(1)); }

	void Adjust(unsigned n)
	{
		for (; vf.size() <= n; )
		{
			vf.push_back(TModular(vf.size()) * vf.back());
			if (keep_inverted)
			{
				vfi.push_back(vf.back().Inverse());
			}
		}
	}

	TModular Get(unsigned n)
	{
		Adjust(n);
		return vf[n];
	}

	TModular GetI(unsigned n)
	{
		Adjust(n);
		if (keep_inverted)
			return vfi[n];
		else
			return vf[n].Inverse();
	}

	TModular operator()(unsigned n) { return Get(n); }

	TModular BinomialCoefficient(unsigned n, unsigned k)
	{
		if (k > n) return 0;
		Adjust(n);
		return keep_inverted ? vf[n] * vfi[k] * vfi[n - k] : vf[n] / (vf[k] * vf[n - k]);
	}
};
