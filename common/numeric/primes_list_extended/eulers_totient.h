#pragma once

#include "../primes_list.h"
#include <vector>

template<class TPrimesList>
class PLEEulersTotient : public TPrimesList
{
protected:
    std::vector<unsigned> eulers_totient;

public:
    PLEEulersTotient(unsigned size) : TPrimesList(size) {}

    void EulersTotientPrecalc()
    {
        eulers_totient.resize(TPrimesList::table_size + 1);
        eulers_totient[0] = 0;
        eulers_totient[1] = 1;
        for (unsigned i = 2; i <= TPrimesList::table_size; ++i)
        {
            unsigned p = TPrimesList::table[i];
            eulers_totient[i] = eulers_totient[i / p] * (TPrimesList::table[i / p] == p ? p : (p - 1));
        }
    }
    
    uint64_t EulersTotientTable(uint64_t n) const
    {
        assert(n <= TPrimesList::table_size);
        uint64_t output = n;
        for (; n > 1; )
		{
			unsigned p = TPrimesList::table[n];
            output -= output / p;
			for (n /= p; TPrimesList::table[n] == p;) n /= p;
		}
        return output;
    }
    
    uint64_t EulersTotient(uint64_t n) const
    {
        if (n < eulers_totient.size()) return eulers_totient[n];
        if (n <= TPrimesList::table_size) return EulersTotientTable(n);
        uint64_t output = n;
        for (uint64_t p : TPrimesList::primes)
        {
			if (n < p * p) break;
			if ((n % p) == 0)
            {
                output -= output / p;
                for (n /= p; (n % p) == 0;) n /= p;
            }
        }
        assert(TPrimesList::squared_table_size > n);
        if (n != 1)
            output -= (output / n);
        return output;
    }
};

using TPLEEulersTotient = PLEEulersTotient<PrimesList>;
