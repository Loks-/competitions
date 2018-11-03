#pragma once

#include "../base.h"

// Return sum of coprime to k numbers in the interval [1..n].
inline uint64_t CoprimeSum(const TFactorization& k_factorization, uint64_t n)
{
    if (k_factorization.size() == 0) return (n*(n+1)) / 2;
    unsigned fs = unsigned(k_factorization.size()), p2 = 1u << fs;
    uint64_t total = 0;
    for (unsigned i = 0; i < p2; ++i)
    {
        uint64_t tn = n, tm = 1;
        unsigned bits = 0;
        for (unsigned j = 0; j < fs; ++j)
        {
            if ((1u << j) & i)
            {
                ++bits;
                tn /= k_factorization[j].first;
                tm *= k_factorization[j].first;
            }
        }
        uint64_t t = tm * ((tn * (tn + 1)) / 2);
        total = ((bits & 1) ? total - t : total + t);
    }
    return total;
}
