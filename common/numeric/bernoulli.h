#pragma once

#include "common/base.h"

#include <vector>

namespace numeric {
template <class TValue, bool positive = false>
class Bernoulli {
 public:
  static constexpr TValue zero = TValue(0), one = TValue(1);

 protected:
  std::vector<TValue> bernoulli, binomial, binomial_temp, inverted;

 public:
  constexpr Bernoulli() { Init(); }

  constexpr void Init() {
    bernoulli.clear();
    binomial.clear();
    inverted.clear();
    constexpr TValue half = one / TValue(2);
    bernoulli.push_back(one);
    bernoulli.push_back(positive ? half : -half);
    binomial.push_back(one);
    binomial.push_back(one);
    inverted.push_back(0);
    inverted.push_back(one);
    inverted.push_back(half);
  }

  constexpr void CalcNext() {
    const unsigned m = unsigned(bernoulli.size());
    assert(binomial.size() == m);
    binomial_temp.swap(binomial);
    binomial_temp.push_back(zero);
    binomial.resize(m + 1);
    binomial[0] = one;
    for (unsigned i = 1; i <= m; ++i)
      binomial[i] = binomial_temp[i - 1] + binomial_temp[i];
    inverted.push_back(one / TValue(inverted.size()));
    if (m & 1) {
      bernoulli.push_back(zero);
      return;
    }
    TValue s = binomial[1] * bernoulli[1] * inverted[m];
    for (unsigned k = 0; k < m; k += 2)
      s += binomial[k] * bernoulli[k] * inverted[m + 1 - k];
    bernoulli.push_back((positive ? one : zero) - s);
  }

  constexpr void Adjust(unsigned n) {
    for (; bernoulli.size() <= n;) CalcNext();
  }

  constexpr TValue Get(unsigned n) {
    Adjust(n);
    return bernoulli[n];
  }

  constexpr TValue operator()(unsigned n) { return Get(n); }

  constexpr TValue GetInverted(unsigned n) {
    assert(n);
    Adjust(n - 1);
    return inverted[n];
  }
};
}  // namespace numeric
