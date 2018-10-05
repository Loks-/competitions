#pragma once

#include "base.h"

// Number of relative prime integers to n.
inline int Mobius(const TFactorization& factorization)
{
    for (auto p : factorization)
        if (p.second > 1)
            return 0;
    return (factorization.size() & 1) ? 1 : -1;
}
