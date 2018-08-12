#pragma once

#include "../base.h"

class PrimesList
{
public:
	vector<uint64_t> vprimes;
	vector<uint64_t> vprimes2; // squared primes

public:
	PrimesList(unsigned max_prime)
	{
		vector<uint8_t> v(max_prime + 1, 1);
		vprimes.push_back(2);
		for (uint64_t i = 3; i <= max_prime; i += 2)
		{
			if (v[i])
			{
				vprimes.push_back(i);
				for (int64_t j = i * i; j <= max_prime; j += 2 * i)
					v[j] = 0;
			}
		}
		vprimes2.reserve(vprimes.size());
		for (size_t i = 0; i < vprimes.size(); ++i)
			vprimes2.push_back(vprimes[i] * vprimes[i]);
	}

	bool IsPrime(uint64_t n)
	{
		if (n <= vprimes.back()) return binary_search(vprimes.begin(), vprimes.end(), n);
		assert(n <= vprimes2.back());
		for (size_t i = 0; vprimes2[i] <= n; ++i)
		{
			if ((n % vprimes[i]) == 0) return false;
		}
		return true;
	}

	vector<pair<uint64_t, unsigned>> Factorize(uint64_t n) const
	{
		vector<pair<uint64_t, unsigned>> output;
		for (size_t i = 0; i < vprimes.size(); ++i)
		{
			if (n < vprimes2[i]) break;
			if ((n % vprimes[i]) == 0)
			{
				n /= vprimes[i];
				unsigned cnt = 1;
				for (; (n % vprimes[i]) == 0; ++cnt)
					n /= vprimes[i];
				output.push_back(make_pair(vprimes[i], cnt));
			}
		}
		assert(vprimes2.back() > n);
		if (n != 1)
			output.push_back(make_pair(n, 1));
		return output;
	}

	// Number of relative prime integers to n.
	uint64_t EulerPhi(uint64_t n) const
	{
		// Factorize function is not callled to make code faster.
		uint64_t output = n;
		for (size_t i = 0; i < vprimes.size(); ++i)
		{
			if (n < vprimes2[i]) break;
			if ((n % vprimes[i]) == 0)
			{
				uint64_t p = vprimes[i];
				output -= (output / p);
				for (n /= p; (n % p) == 0;) 
					n /= p;
			}
		}
		assert(vprimes2.back() > n);
		if (n != 1)
			output -= (output / n);
		return output;
	}

	unsigned GetDivisorsCount(uint64_t n) const
	{
		auto vp = Factorize(n);
		unsigned k = 1;
		for (auto p : vp)
			k *= (p.second + 1);
		return k;
	}

protected:
	void GetDivisorsI(const vector<pair<uint64_t, unsigned>>& vfactorization, unsigned fpi, uint64_t current, vector<uint64_t>& output) const
	{
		if (fpi < vfactorization.size())
		{
			for (unsigned i = 0; i <= vfactorization[fpi].second; ++i)
			{
				GetDivisorsI(vfactorization, fpi + 1, current, output);
				current *= vfactorization[fpi].first;
			}
		}
		else
			output.push_back(current);
	}

public:
	vector<uint64_t> GetDivisors(uint64_t n) const
	{
		vector<uint64_t> v;
		GetDivisorsI(Factorize(n), 0, 1, v);
		return v;
	}
};
