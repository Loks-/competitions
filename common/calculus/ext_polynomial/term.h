#pragma once

#include "common/calculus/ext_polynomial/term_power.h"

namespace calculus {
namespace ext_polynomial {
template <class TValue, class TTermPower = TermPower<TValue>>
class Term {
 public:
  using TSelf = Term<TValue, TTermPower>;

  TValue a;
  TTermPower tp;

  Term(const TValue& _a) : a(_a) {}
  Term(const TValue& _a, const TTermPower& _tp) : a(_a), tp(_tp) {}

  bool IsConstant() const { return tp.IsConstant(); }
  bool IsPolynomial() const { return tp.IsPolynomial(); }

  TValue Get(const TValue& x) const { return a * tp.Get(x); }

  TSelf operator-() const { return TSelf(-a, tp); }

  TSelf& operator*=(const TValue& r) {
    a *= r;
    return *this;
  }

  TSelf operator*(const TValue& r) const { return TSelf(a * r, tp); }

  bool IsMultiplicable(const TSelf& r) const {
    return tp.IsMultiplicable(r.tp);
  }

  TSelf& operator*=(const TSelf& r) {
    a *= r.a;
    tp *= r.tp;
    return *this;
  }

  TSelf operator*(const TSelf& r) const {
    TSelf t(*this);
    t *= r;
    return t;
  }

  TSelf& operator/=(const TValue& r) {
    a /= r;
    return *this;
  }

  TSelf operator/(const TValue& r) const { return TSelf(a / r, tp); }

  bool IsDivisible(const TSelf& r) const { return tp.IsDivisible(r.tp); }

  TSelf& operator/=(const TSelf& r) {
    a /= r.a;
    tp /= r.tp;
    return *this;
  }

  TSelf operator/(const TSelf& r) const {
    TSelf t(*this);
    t /= r;
    return t;
  }
};

template <class TValue, class TTermPower>
inline Term<TValue, TTermPower> operator*(const TValue& l,
                                          const Term<TValue, TTermPower>& r) {
  return r * l;
}

template <class TValue, class TTermPower>
inline Term<TValue, TTermPower> operator/(const TValue& l,
                                          const Term<TValue, TTermPower>& r) {
  return Term<TValue, TTermPower>(l) / r;
}

using DTerm = Term<double, DTermPower>;
}  // namespace ext_polynomial
}  // namespace calculus
