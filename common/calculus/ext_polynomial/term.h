#pragma once

#include "common/calculus/ext_polynomial/term_power.h"
#include <string>

namespace calculus {
namespace ext_polynomial {
template <class TValueF, class TValueTerm = TValueF,
          class TTermPower = TermPower<TValueTerm>>
class Term {
 public:
  using TSelf = Term<TValueF, TValueTerm, TTermPower>;

  TValueF a;
  TTermPower tp;

  Term(const TValueF& _a) : a(_a) {}
  Term(const TValueF& _a, const TTermPower& _tp) : a(_a), tp(_tp) {}

  bool IsConstant() const { return tp.IsConstant(); }
  bool IsPolynomial() const { return tp.IsPolynomial(); }

  TValueF Get(const TValueTerm& x) const {
    return (a != TValueF(0)) ? a * TValueF(tp.Get(x)) : TValueF(0);
  }

  bool IsFinite(const TValueTerm& x) const {
    return (a == TValueF(0)) || tp.IsFinite(x);
  }

  bool operator==(const TSelf& r) const { return (a == r.a) && (tp == r.tp); }

  std::string ToString(const std::string& variable_name) const {
    if (a == TValueF(0)) return "";
    std::string stp = tp.ToString(variable_name);
    if (stp.empty()) {
      if (a < TValueF(0)) {
        return "-" + std::to_string(-a);
      } else {
        return "+" + std::to_string(a);
      }
    } else if (a == TValueF(1)) {
      return "+" + stp;
    } else if (a == -TValueF(1)) {
      return "-" + stp;
    } else if (a < TValueF(0)) {
      return "-" + std::to_string(-a) + "*" + stp;
    } else {
      return "+" + std::to_string(a) + "*" + stp;
    }
  }

  TSelf operator-() const { return TSelf(-a, tp); }

  TSelf& operator*=(const TValueF& r) {
    a *= r;
    return *this;
  }

  TSelf operator*(const TValueF& r) const { return TSelf(a * r, tp); }

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

  TSelf& operator/=(const TValueF& r) {
    a /= r;
    return *this;
  }

  TSelf operator/(const TValueF& r) const { return TSelf(a / r, tp); }

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

template <class TValueF, class TValueTerm, class TTermPower>
inline Term<TValueF, TValueTerm, TTermPower> operator*(
    const TValueF& l, const Term<TValueF, TValueTerm, TTermPower>& r) {
  return r * l;
}

template <class TValueF, class TValueTerm, class TTermPower>
inline Term<TValueF, TValueTerm, TTermPower> operator/(
    const TValueF& l, const Term<TValueF, TValueTerm, TTermPower>& r) {
  return Term<TValueF, TValueTerm, TTermPower>(l) / r;
}

using DTerm = Term<double, double, DTermPower>;
}  // namespace ext_polynomial
}  // namespace calculus
