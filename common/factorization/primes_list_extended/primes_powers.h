#pragma once

#include "../primality_test.h"
#include "../primes_list.h"
#include "../../numeric/utils/usqrt.h"
#include <vector>

class PLEPrimesPowers : public PrimesList
{
protected:
    MillerRabinPrimalityTest primality_test;

public:
    PLEPrimesPowers(unsigned size = 3000000) : PrimesList(size) {}

    std::vector<unsigned> GetPrimesPowers(uint64_t n) const
    {
        if (n <= 1) return {};
        TFactorization f = PrimesList::Factorize(n, true);
        std::vector<unsigned> output;
        for (auto p : f)
            output.push_back(p.second);
        n = f.back().first;
        if (n <= PrimesList::squared_table_size) return output;
        if (primality_test.IsPrime(n)) return output;
        uint64_t nsqrt = USqrt(n);
        if (nsqrt * nsqrt == n)
            output.back() = 2;
        else
            output.push_back(1);
        return output;
    }
};
