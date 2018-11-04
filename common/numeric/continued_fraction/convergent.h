#pragma once

#include "continued_fraction.h"
#include "../fraction.h"

class CFConvergent
{
protected:
    ContinuedFraction cf;
    unsigned itc;
    int64_t n0, n1, d0, d1;

public:
    void ResetIterator()
    {
        itc = 0;
        n0 = cf(0); n1 = 1;
        d0 = 1; d1 = 0;
    }

    CFConvergent(const ContinuedFraction& continued_fraction) : cf(continued_fraction)
    {
        ResetIterator();
    }

    bool End() const { return (itc + 1) >= cf.Size(); }
    TIFraction Get() const { return TIFraction(n0, d0); }
    
    CFConvergent& Next()
    {
        ++itc;
        int64_t nn = cf(itc) * n0 + n1;
        n1 = n0; n0 = nn;
        int64_t dn = cf(itc) * d0 + d1;
        d1 = d0; d0 = dn;
        return *this;
    }

    TIFraction operator*() const { return Get(); }
    CFConvergent& operator++() { return Next(); }
};
