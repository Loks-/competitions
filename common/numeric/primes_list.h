#pragma once

#include "factorization.h"
#include "../base.h"
#include <algorithm>
#include <vector>
#include <utility>

class PrimesList
{
protected:
	uint64_t table_size, squared_table_size;
	std::vector<uint64_t> primes, squared_primes;

public:
	PrimesList(unsigned size)
	{
		table_size = size;
		squared_table_size = table_size * table_size;
		std::vector<uint8_t> v(table_size + 1, 1);
		primes.push_back(2);
		for (uint64_t i = 3; i <= table_size; i += 2)
		{
			if (v[i])
			{
				primes.push_back(i);
				for (uint64_t j = i * i; j <= table_size; j += 2 * i)
					v[j] = 0;
			}
		}
		squared_primes.reserve(primes.size());
		for (size_t i = 0; i < primes.size(); ++i)
			squared_primes.push_back(primes[i] * primes[i]);
	}

	const std::vector<uint64_t>& GetPrimes() const { return primes; }
	const std::vector<uint64_t>& GetSquaredPrimes() const { return squared_primes; }
	uint64_t GetTableSize() const { return table_size; }
	uint64_t GetSquaredTableSize() const { return squared_table_size; }

	bool IsPrime(uint64_t n)
	{
		assert(n <= squared_table_size);
		if (n <= table_size) return std::binary_search(primes.begin(), primes.end(), n);
		for (size_t i = 0; squared_primes[i] <= n; ++i)
		{
			if ((n % primes[i]) == 0) return false;
		}
		return true;
	}

	TFactorization Factorize(uint64_t n) const
	{
		TFactorization output;
		for (size_t i = 0; i < primes.size(); ++i)
		{
			if (n < squared_primes[i]) break;
			if ((n % primes[i]) == 0)
			{
				uint64_t p = primes[i];
				n /= p;
				unsigned cnt = 1;
				for (; (n % p) == 0; ++cnt) n /= p;
				output.push_back(std::make_pair(p, cnt));
			}
		}
		assert(squared_table_size > n);
		if (n != 1)
			output.push_back(std::make_pair(n, 1));
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
	void GetDivisorsI(const TFactorization& factorization, unsigned fpi, uint64_t current, std::vector<uint64_t>& output) const
	{
		if (fpi < factorization.size())
		{
			for (unsigned i = 0; i <= factorization[fpi].second; ++i)
			{
				GetDivisorsI(factorization, fpi + 1, current, output);
				current *= factorization[fpi].first;
			}
		}
		else
			output.push_back(current);
	}

public:
	std::vector<uint64_t> GetDivisors(uint64_t n) const
	{
		std::vector<uint64_t> v;
		GetDivisorsI(Factorize(n), 0, 1, v);
		return v;
	}
};
