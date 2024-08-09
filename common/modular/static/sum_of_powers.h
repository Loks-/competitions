#pragma once

#include "common/modular/static/factorial.h"
#include "common/numeric/bernoulli.h"

// Based on Bernoulli numbers
// ( https://en.wikipedia.org/wiki/Faulhaber's_formula ) .
namespace modular {
namespace mstatic {
template <class TModular>
class SumOfPowers {
 public:
  using TBernoulli = numeric::Bernoulli<TModular, true>;
  using TFactorial = Factorial<TModular, true>;

 protected:
  TBernoulli b;
  TFactorial f;

 public:
  TModular Sum(uint64_t n, unsigned power) {
    if (power == 0) return TModular(n);
    if (power == 1) return TModular((n + 1) / 2) * TModular(n | 1);
    b.Adjust(power);
    f.Adjust(power + 1);
    const TModular x(n), xx = x * x;
    TModular s = (b(0) * x + b(1) * TModular(power + 1)) * x +
                 b(2) * f.BinomialCoefficient(power + 1, 2);
    for (unsigned j = 4; j <= power; j += 2)
      s = s * xx + b(j) * f.BinomialCoefficient(power + 1, j);
    s *= (power & 1 ? xx : x);
    return s * b.GetInverted(power + 1);
  }
};
}  // namespace mstatic
}  // namespace modular
