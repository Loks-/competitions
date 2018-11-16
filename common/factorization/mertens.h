#pragma once

#include "primes_list_extended/mertens.h"
#include "../numeric/utils/usqrt.h"

class Mertens
{
protected:
    uint64_t u;
    TPLEMertens mertens;

    int S(uint64_t y) const
    {
        uint64_t v = USqrt(y), k = y / (v + 1);
        int r = 1 + int(k) * mertens.Mertens(v);
        for (uint64_t n = y / u + 1; n <= k; ++n)
            r -= mertens.Mertens(y / n);
        for (uint64_t n = 1; n <= v; ++n)
            r -= int(y / n) * mertens.Mobius(n);
        return r;
    }

public:
    Mertens(unsigned _u) : u(_u), mertens(u) { mertens.PrecalcMertens(); }

    int GetMobius(uint64_t x) const { return mertens.Mobius(x); }

    int GetMertens(uint64_t x) const
    {
        if (x <= u) return mertens.Mertens(x);
        int r = 0;
        for (uint64_t n = 1; n <= x / u; ++n)
        {
            int m = mertens.Mobius(n);
            if (m) r += m * S(x / n);
        }
        return r;
    }

    int operator()(uint64_t x) const { return GetMertens(x); }
};
