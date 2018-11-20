#pragma once

#include "mobius.h"
#include "../primes_list.h"
#include "../../base.h"
#include <vector>

template<class TPrimesList>
class PLEMertens : public PLEMobius<TPrimesList>
{
public:
    using TBase = PLEMobius<TPrimesList>;
    using TSelf = PLEMertens<TPrimesList>;

protected:
    std::vector<int> mertens;

public:
    PLEMertens(uint64_t size) : TBase(size) {}

    void PrecalcMertens()
    {
        if (TBase::mobius.size() == 0) TBase::PrecalcMobius();
        assert(TBase::mobius.size() == TBase::table_size + 1);
        mertens.resize(TBase::table_size + 1);
        mertens[0] = 0;
        for (unsigned i = 1; i <= TBase::table_size; ++i)
            mertens[i] = mertens[i-1] + TBase::mobius[i];
    }
    
    int Mertens(uint64_t n) const { assert(n < mertens.size()); return mertens[n]; }
};

using TPLEMertens = PLEMertens<PrimesList>;
