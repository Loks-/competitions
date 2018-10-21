#pragma once

#include "factorial_modular.h"
#include "../numeric/bernoulli.h"

// Based on Bernoulli numbers ( https://en.wikipedia.org/wiki/Faulhaber's_formula ) .
template <class TModular>
class SumOfPowersModular
{
public:
    using TBernoulli = Bernoulli<TModular, true>;
    using TFactorial = FactorialModular<TModular, true>;

protected:
    TBernoulli b;
    TFactorial f;

public:
    TModular Sum(uint64_t n, unsigned power)
    {
        if (power == 0) return TModular(n);
        if (power == 1) return TModular((n + 1) / 2) * TModular(n | 1);
        b.Adjust(power); f.Adjust(power + 1);
        TModular x(n), xx = x*x;
        TModular s = (b(0) * x + b(1) * TModular(power + 1)) * x + b(2) * f.BinomialCoefficient(power + 1, 2);
        for (unsigned j = 4; j <= power; j += 2)
            s = s * xx + b(j) * f.BinomialCoefficient(power + 1, j);
        s *= (power & 1 ? xx : x);    
        return s * b.GetInverted(power + 1);
    }
};
