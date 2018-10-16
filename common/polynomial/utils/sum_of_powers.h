#pragma once

#include "../base_newton_polynomial.h"
#include <vector>

template<class TValue>
inline BaseNewtonPolynomial<TValue> GetSumOfPowers(unsigned power)
{
    std::vector<TValue> vy(power + 2, 0);
    for (unsigned i = 1; i < vy.size(); ++i)
        vy[i] = vy[i - 1] + TValue(i).PowU(power);
    BaseNewtonPolynomial<TValue> r;
    r.Interpolate(vy);
    return r;
}
