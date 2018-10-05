#pragma once

#include "primes_list.h"
#include <vector>

// Support Euler's Totient function.
// Factorize function is not callled to make code faster.
class PrimesListExtended : public PrimesList
{
protected:
    std::vector<unsigned> eulers_totient;

public:
    PrimesListExtended(unsigned size) : PrimesList(size) {}

    void EulersTotientPrecalc()
    {
        eulers_totient.resize(PrimesList::table_size + 1);
        eulers_totient[0] = 0;
        eulers_totient[1] = 1;
        for (unsigned i = 2; i <= PrimesList::table_size; ++i)
        {
            unsigned p = PrimesList::table[i];
            eulers_totient[i] = eulers_totient[i / p] * (PrimesList::table[i / p] == p ? p : (p - 1));
        }
    }
    
    uint64_t EulersTotientTable(uint64_t n) const
    {
        assert(n <= PrimesList::table_size);
        uint64_t output = n;
        for (; n > 1; )
		{
			unsigned p = PrimesList::table[n];
            output -= output / p;
			for (n /= p; PrimesList::table[n] == p;) n /= p;
		}
        return output;
    }
    
    uint64_t EulersTotient(uint64_t n) const
    {
        if (n < eulers_totient.size()) return eulers_totient[n];
        if (n <= PrimesList::table_size) return EulersTotientTable(n);
        uint64_t output = n;
        for (uint64_t p : PrimesList::primes)
        {
			if (n < p * p) break;
			if ((n % p) == 0)
            {
                output -= output / p;
                for (n /= p; (n % p) == 0;) n /= p;
            }
        }
        assert(PrimesList::squared_table_size > n);
        if (n != 1)
            output -= (output / n);
        return output;
    }
};
