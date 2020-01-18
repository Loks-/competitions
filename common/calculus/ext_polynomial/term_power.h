#pragma once

#include "common/calculus/ext_polynomial/term_base.h"
#include "common/calculus/ext_polynomial/term_bases/one.h"
#include "common/calculus/ext_polynomial/term_bases/type.h"
#include "common/numeric/utils/pow.h"
#include <memory>
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
  TermPower() : base(term_bases::MakeOne<TValue>()), power(0) {}
  TermPower(TTermBase _base, int _power = 0) : base(_base), power(_power) {}

  term_bases::Type GetType() const { return base->GetType(); }
  bool IsConstant() const { return (power == 0) && base->IsOne(); }
  bool IsPolynomial() const { return base->IsOne(); }

  TValue Get(const TValue& x) const { return base->Get(x) * PowS(x, power); }

  bool operator<(const TSelf& r) const {
    return (power < r.power) ? true : (r.power < power) ? false
                                                        : (*base < *r.base);
  }

  bool operator==(const TSelf& r) const {
    return (power == r.power) && (*base == *r.base);
  }

  bool ValidMultiplication(const TSelf& r) const {
    return base->ValidMultiplication(*r.base);
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

  bool ValidDivision(const TSelf& r) const {
    return base->ValidDivision(*r.base);
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
}  // namespace ext_polynomial
}  // namespace calculus
