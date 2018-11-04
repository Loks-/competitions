#pragma once

#include "continued_fraction.h"
#include "convergent.h"
#include "../fraction.h"

#include <iostream>

class CFSemiConvergent : public CFConvergent
{
public:
    using TBase = CFConvergent;

protected:
    int64_t its;

    int64_t ItsEnd() const { return TBase::cf(TBase::itc + 1); }

    bool IsHalfValid() const { return false; } // fix
    void AdjustSemiIterator() { its = ItsEnd() / 2; if ((ItsEnd() & 1) || !IsHalfValid()) ++its; }

public:
    void ResetIterator()
    {
        TBase::ResetIterator();
        its = ItsEnd() / 2 + 1;
    }

    CFSemiConvergent(const ContinuedFraction& continued_fraction) : TBase(continued_fraction)
    {
        ResetIterator();
    }

    bool End() const { return ((TBase::itc + 2) >= TBase::cf.Size()) && (its == ItsEnd()); }
    TIFraction Get() const { return TIFraction(its * TBase::n0 + TBase::n1, its * TBase::d0 + TBase::d1); }
    
    CFSemiConvergent& Next()
    {
        if (its == ItsEnd())
        {
            TBase::Next();
            AdjustSemiIterator();
        }
        else
            ++its;
        return *this;
    }

    TIFraction operator*() const { return Get(); }
    CFSemiConvergent& operator++() { return Next(); }
};
