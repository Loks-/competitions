#pragma once

#include "../base.h"
#include "fraction.h"
#include <limits>
#include <vector>

class ContinuedFraction
{
protected:
    std::vector<int64_t> cf;

    int64_t it1, it2;
    int64_t n0, n1, d0, d1;

    void IncreaseIT1()
    {
        int64_t nn = cf[it1] * n0 + n1;
        n1 = n0; n0 = nn;
        int64_t dn = cf[it1] * d0 + d1;
        d1 = d0; d0 = dn;
        ++it1;
    }

    bool HalfRule() const
    { 
        for (int64_t i = 1; i < it1; ++i)
        {
            int64_t l = cf[it1 - i];
            int64_t r = ((it1 + i) < int64_t(cf.size())) ? cf[it1 + i] : std::numeric_limits<int64_t>::max();
            if (l != r)
                return (l < r) != (i & 1);
        }
        return (it1 & 1) == 0;
    }

public:
    ContinuedFraction(const std::vector<int64_t>& continued_fraction) : cf(continued_fraction)
    {
        it1 = 0; it2 = cf[0] - 1;
        n0 = 1; n1 = 0;
        d0 = 0; d1 = 1;
    }

    TIFraction GetCurrentFraction() { return TIFraction(it2 * n0 + n1, it2 * d0 + d1); }
    
    TIFraction GetNextFraction() 
    { 
        if (it2 == cf[it1])
        {
            IncreaseIT1();
            it2 = cf[it1] / 2;
            if (((cf[it1] & 1) == 0) && HalfRule())
                --it2;
        }
        ++it2;
        return GetCurrentFraction();
    }
    
    TIFraction GetNextBestFraction()
    {
        if (it2 == cf[it1])
            IncreaseIT1();
        it2 = cf[it1];
        return GetCurrentFraction();
    }
};
