#pragma once

#include "../base.h"

// Number of relative prime integers to n.
inline uint64_t EulersTotient(uint64_t n, const TFactorization& factorization)
{
    uint64_t output = n;
    for (auto p : factorization)
        output -= output / p.first;
    return output;
}
