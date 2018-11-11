#pragma once

#include <cmath>
#include <vector>

class FactorialDouble
{
protected:
	std::vector<double> vf;

public:
	FactorialDouble() { vf.push_back(0.); }

	void Adjust(unsigned n)
	{
		for (; vf.size() <= n; )
			vf.push_back(log(vf.size()) + vf.back());
	}

	double GetLog(unsigned n) { Adjust(n); return vf[n]; }
	double Get(unsigned n) { return exp(GetLog(n)); }

	double BinomialCoefficientLog(unsigned n, unsigned k)
	{
		if (k > n) return -INFINITY;
		Adjust(n);
		return vf[n] - (vf[k] + vf[n - k]);
	}

	double BinomialCoefficient(unsigned n, unsigned k)
	{
		if (k > n) return 0.;
		return exp(BinomialCoefficientLog(n, k));
	}
};
