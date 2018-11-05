#pragma once

#include "continued_fraction.h"
#include "convergent.h"
#include "../fraction.h"

class CFSemiConvergent : public CFConvergent
{
public:
    using TBase = CFConvergent;
    using TCFVector = ContinuedFraction::TCFVector;

protected:
    int64_t its;

    int64_t ItsLast() const { return End() ? 1 : TBase::cf(TBase::itc); }
    void AdjustSemiIterator() { its = ItsLast() / 2; if ((ItsLast() & 1) || !IsHalfValid()) ++its; }

    bool IsHalfValid() const
    {
        const std::vector<int64_t>& v = TBase::cf.GetVector();
        ContinuedFraction cfl(TCFVector(v.begin() + TBase::itc, v.end()));
        ContinuedFraction cfr(TCFVector(v.rend() - TBase::itc - 1, v.rend() - 1));
        return cfl < cfr;
    }

public:
    void ResetIterator()
    {
        TBase::ResetIteratorI();
        its = ItsLast();
    }

    CFSemiConvergent(const ContinuedFraction& continued_fraction) : TBase(continued_fraction)
    {
        ResetIterator();
    }

    bool End() const { return TBase::itc >= TBase::cf.Size(); }
    TIFraction Get() const { return TIFraction(its * TBase::n0 + TBase::n1, its * TBase::d0 + TBase::d1); }
    
    CFSemiConvergent& Next()
    {
        if (its == ItsLast())
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
