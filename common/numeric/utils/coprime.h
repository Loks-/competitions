#pragma once

#include "../primes_list.h"
#include <vector>
#include <utility>

// Return number of coprime to k numbers in interval 1..n .
inline uint64_t CoprimeNumers(const std::vector<std::pair<uint64_t, unsigned>>& kf, uint64_t n)
{
    if (kf.size() == 0) return n;
    unsigned fs = unsigned(kf.size()), p2 = 1u << fs;
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
                t /= kf[j].first;
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
