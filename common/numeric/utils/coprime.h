#pragma once

#include "../primes_list.h"

// Return number of coprime to k numbers in the interval [1..n].
inline uint64_t CoprimeNumers(const PrimesList::TFactorization& k_factorization, uint64_t n)
{
    if (k_factorization.size() == 0) return n;
    unsigned fs = unsigned(k_factorization.size()), p2 = 1u << fs;
    uint64_t total = 0;
    for (unsigned i = 0; i < p2; ++i)
    {
        uint64_t t = n;
        unsigned bits = 0;
        for (unsigned j = 0; j < fs; ++j)
        {
            if ((1u << j) & i)
            {
                ++bits;
                t /= k_factorization[j].first;
            }
        }
        total = ((bits & 1) ? total - t : total + t);
    }
    return total;
}

inline uint64_t CoprimeNumers(uint64_t k, uint64_t n, const PrimesList& primes_list)
{
    return CoprimeNumers(primes_list.Factorize(k), n);
}
