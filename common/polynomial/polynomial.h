#pragma once

#include "common/polynomial/base.h"

#include <vector>

namespace polynomial {
template <class TTValue>
class Polynomial : public Base<TTValue, Polynomial<TTValue>> {
 public:
  using TValue = TTValue;
  using TSelf = Polynomial<TValue>;
  using TBase = Base<TValue, TSelf>;

  Polynomial() {}

  explicit Polynomial(const TValue& v) {
    TBase::data.resize(1, v);
    TBase::Normalize();
  }

  Polynomial(const TValue& v0, const TValue& v1) {
    TBase::data.resize(2);
    TBase::data[0] = v0;
    TBase::data[1] = v1;
    TBase::Normalize();
  }

  TValue Apply(TValue x) const {
    if (TBase::Empty()) return TValue();
    TValue *p = TBase::end(), *pbegin = TBase::begin();
    TValue r = *(--p);
    for (; p != pbegin;) r = r * x + *(--p);
    return r;
  }

  TValue operator()(const TValue& x) const { return Apply(x); }

  // Multiply by x^n
  void MultXN(unsigned n) {
    if (!TBase::Empty()) TBase::data.insert(TBase::data.begin(), n, TValue());
  }

  void MultX() { MultXN(1u); }
};
}  // namespace polynomial
