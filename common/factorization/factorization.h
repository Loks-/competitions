#pragma once

#include "base.h"
#include "primality_test.h"
#include "../modular/modular_arithmetic_proxy.h"
#include "../numeric/utils/gcd.h"
#include <algorithm>
#include <vector>

// Based on Pollard's rho algorithm ( https://en.wikipedia.org/wiki/Pollard%27s_rho_algorithm ).
class Factorization
{
protected:
    const uint64_t u32threshold = (1u << 31);
	MillerRabinPrimalityTest primality_test;

	uint64_t FindFactor32(uint64_t n, uint64_t start)
	{
		ModularArithmeticProxy<false, true> proxy(n);
		auto next = [&](uint64_t k) { return proxy.Add(proxy.Mult(k, k), 1); };
		uint64_t x = start, y = start, d = 1;
		for (; d == 1;)
		{
			x = next(x);
			y = next(next(y));
			d = GCD(proxy.Sub(x, y), n);
		}
		return d;
	}

	uint64_t FindFactor64(uint64_t n, uint64_t start)
	{
		ModularArithmeticProxy<false, false> proxy(n);
		auto next = [&](uint64_t k) { return proxy.Add(proxy.Mult(k, k), 1); };
		uint64_t x = start, y = start, d = 1;
		for (; d == 1;)
		{
			x = next(x);
			y = next(next(y));
			d = GCD(proxy.Sub(x, y), n);
		}
		return d;
	}

	uint64_t FindFactor(uint64_t n)
	{
		uint64_t d = n;
		for (uint64_t start = 2; d == n; ++start)
			d = (n < u32threshold) ? FindFactor32(n, start) : FindFactor64(n, start);
		return d;
	}

	std::vector<uint64_t> FactorizeI(uint64_t n)
	{
		if (n == 1) return {};
		std::vector<uint64_t> v;
		for (; !primality_test.IsPrime(n); )
		{
			uint64_t d = FindFactor(n);
			n /= d;
			if (primality_test.IsPrime(d))
				v.push_back(d);
			else
			{
				auto vd = FactorizeI(d);
				v.insert(v.end(), vd.begin(), vd.end());
			}
		}
		if (n > 1)
			v.push_back(n);
		return v;
	}

	std::vector<uint64_t> FactorizeISafe(uint64_t n)
	{
		std::vector<uint64_t> v;
		for (uint64_t p : {2, 3, 5})
		{
			for (; (n % p) == 0; n /= p)
				v.push_back(p);
		}
		auto vv = FactorizeI(n);
		v.insert(v.end(), vv.begin(), vv.end());
		return v;
	}

public:
	TFactorization Factorize(uint64_t n)
	{
		std::vector<uint64_t> v = FactorizeISafe(n);
		std::sort(v.begin(), v.end());
		TFactorization f;
		for (uint64_t p : v)
		{
			if ((f.size() == 0) || (f.back().first != p))
				f.push_back({ p, 1 });
			else
				f.back().second += 1;
		}
		return f;
	}
};

inline TFactorization FastFactorize(uint64_t n) { Factorization f; return f.Factorize(n); } 
