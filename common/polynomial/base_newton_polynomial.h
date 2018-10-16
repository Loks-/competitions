#pragma once

#include "base_polynomial.h"
#include <vector>

// Basis polynomials:
//   f_k = x * (x - 1) * ... * (x - k + 1)
template<class TTValue>
class BaseNewtonPolynomial : BasePolynomial<TTValue, BaseNewtonPolynomial<TTValue>>
{
public:
    using TValue = TTValue;
    using TSelf = BaseNewtonPolynomial<TValue>;
    using TBase = BasePolynomial<TValue, TSelf>;

    TValue Apply(TValue x) const
    {
        if (TBase::Size() <= 1) return TBase::GetCoefficient(0);
        const TValue *p = TBase::begin(), *pend = TBase::end();
        TValue r = *p++, f = x, one = TValue(1);
        for (; p < pend; )
        {
            r += *p++ * f;
            x -= one;
            f *= x;
        }
        return r;
    }

    TValue operator()(const TValue& x) const { return Apply(x); }

    TSelf& Interpolate(const std::vector<TValue>& vy)
    {
        TBase::Clear();
        std::vector<TValue> v(vy);
        unsigned step = 0;
        for (; v.size(); )
        {
            TBase::data.push_back(v[0]);
            step += 1;
            TValue step_inverted = TValue(1) / TValue(step);
            for (unsigned i = 1; i < v.size(); ++i)
                v[i-1] = (v[i] - v[i-1]) * step_inverted;
            v.pop_back();
        }
        return *this;
    }
};
