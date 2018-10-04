#pragma once

#include "../primes_list.h"
#include <vector>

// Number of relative prime integers to n.
inline uint64_t EulersTotient(uint64_t n, const PrimesList& primes_list)
{
    // Factorize function is not callled to make code faster.
    const std::vector<uint64_t>& primes = primes_list.GetPrimes();
    const std::vector<uint64_t>& squared_primes = primes_list.GetSquaredPrimes();
    uint64_t output = n;
    for (size_t i = 0; i < primes.size(); ++i)
    {
        if (n < squared_primes[i]) break;
        if ((n % primes[i]) == 0)
        {
            uint64_t p = primes[i];
            output -= output / p;
            for (n /= p; (n % p) == 0;) n /= p;
        }
    }
    assert(primes_list.GetSquaredTableSize() > n);
    if (n != 1)
        output -= (output / n);
    return output;
}
