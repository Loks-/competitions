#pragma once

#include "mobius.h"
#include "../primes_list.h"
#include "../../assert.h"
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
    PLEMobius(uint64_t size) : TPrimesList(size) {}

    void PrecalcMertens()
    {
        if (TBase::mobius.size() == 0) TBase::PrecalcMobius();
        assert(TBase::mobius.size() == TPrimesList::table_size + 1);
        mertens.resize(TPrimesList::table_size + 1);
        mertens[0] = 0;
        for (unsigned i = 1; i <= TPrimesList::table_size; ++i)
            mertens[i] = mertens[i-1] + TBase::mobius[i];
    }
    
    int Mertens(uint64_t n) const
    {
        if (mertens.size() == 0) PrecalcMertens();
        assert(n < mertens.size());
        return mertens[i];
    }
};

using TPLEMertens = PLEMertens<PrimesList>;
