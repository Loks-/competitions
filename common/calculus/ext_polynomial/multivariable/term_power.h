#pragma once

#include "common/calculus/ext_polynomial/term_power.h"
#include "common/stl/array.h"
#include <array>

namespace calculus {
namespace ext_polynomial {
namespace multivariable {
template <class TValue>
using SVTermPower = calculus::ext_polynomial::TermPower<TValue>;

template <class TValue, unsigned _dim>
class TermPower {
 public:
  using TTermPowerSV = SVTermPower<TValue>;
  using TSelf = TermPower<TValue, _dim>;
  static const unsigned dim = _dim;

  std::array<TTermPowerSV, _dim> terms;

  TermPower() : terms(MakeArray<TTermPowerSV, dim>(TTermPowerSV())) {}
  TermPower(const std::array<TTermPowerSV, _dim>& _terms) : terms(_terms) {}

  bool IsPolynomial() const {
    for (auto& t : terms) {
      if (!t.IsPolynomial()) return false;
    }
    return true;
  }

  bool IsConstant() const {
    for (auto& t : terms) {
      if (!t.IsConstant()) return false;
    }
    return true;
  }

  bool UnusedVariable(unsigned index) const {
    return terms[index].IsConstant();
  }

  TTermPowerSV& operator()(unsigned index) { return terms[index]; }
  const TTermPowerSV& operator()(unsigned index) const { return terms[index]; }
  bool operator<(const TSelf& r) const { return terms < r.terms; }
  bool operator==(const TSelf& r) const { return terms == r.terms; }
};
}  // namespace multivariable
}  // namespace ext_polynomial
}  // namespace calculus
