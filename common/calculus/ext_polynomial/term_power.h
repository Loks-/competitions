#pragma once

#include "common/base.h"
#include "common/calculus/ext_polynomial/term_base.h"
#include "common/calculus/ext_polynomial/term_bases/one.h"
#include "common/calculus/ext_polynomial/term_bases/type.h"

#include <string>

namespace calculus {
namespace ext_polynomial {
template <class TValue>
class TermPower {
 public:
  using TTermBase = PTermBase<TValue>;
  using TSelf = TermPower<TValue>;

 public:
  TTermBase base;
  int power;

 public:
  constexpr TermPower() : base(term_bases::MakeOne<TValue>()), power(0) {}

  constexpr TermPower(TTermBase _base, int _power = 0)
      : base(_base), power(_power) {
    assert(base);
  }

  constexpr term_bases::Type GetType() const { return base->GetType(); }

  constexpr bool IsConstant() const { return (power == 0) && base->IsOne(); }

  constexpr bool IsPolynomial() const { return base->IsOne(); }

  constexpr TValue Get(const TValue& x) const { return base->Get(x, power); }

  constexpr bool IsFinite(const TValue& x) const {
    return base->IsFinite(x, power);
  }

  constexpr bool operator<(const TSelf& r) const {
    return (power < r.power)   ? true
           : (r.power < power) ? false
                               : (*base < *r.base);
  }

  constexpr bool operator==(const TSelf& r) const {
    return (power == r.power) && (*base == *r.base);
  }

  constexpr bool IsMultiplicable(const TSelf& r) const {
    return base->IsMultiplicable(*r.base);
  }

  TSelf& operator*=(const TSelf& r) {
    base = base->Multiplication(base, r.base);
    power += r.power;
    return *this;
  }

  TSelf operator*(const TSelf& r) const {
    TSelf t(*this);
    t *= r;
    return t;
  }

  constexpr bool IsDivisible(const TSelf& r) const {
    return base->IsDivisible(*r.base);
  }

  TSelf& operator/=(const TSelf& r) {
    base = base->Division(base, r.base);
    power -= r.power;
    return *this;
  }

  TSelf operator/(const TSelf& r) const {
    TSelf t(*this);
    t /= r;
    return t;
  }

  std::string ToString(const std::string& variable_name) const {
    std::string xpk;
    if (power != 0) {
      xpk = variable_name;
      if (power != 1) {
        xpk = xpk + "^" + ((power < 0) ? "(" : "") + std::to_string(power) +
              ((power < 0) ? ")" : "");
      }
    }
    std::string tbname = base->ToString(variable_name);
    return xpk + ((tbname.empty() || xpk.empty()) ? "" : "*") + tbname;
  }
};

using DTermPower = TermPower<double>;
}  // namespace ext_polynomial
}  // namespace calculus
