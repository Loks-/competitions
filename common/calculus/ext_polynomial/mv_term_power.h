#pragma once

#include "common/calculus/ext_polynomial/term_power.h"
#include "common/calculus/multivariable/point.h"
#include "common/stl/array.h"

#include <array>
#include <string>

namespace calculus {
namespace ext_polynomial {
template <class TValue, unsigned dim>
class MVTermPower {
 public:
  using TPoint = calculus::multivariable::Point<TValue, dim>;
  using TSVTermPower = TermPower<TValue>;
  using TSelf = MVTermPower<TValue, dim>;

  static constexpr unsigned Dim() { return dim; }

 public:
  std::array<TSVTermPower, dim> terms;

 public:
  constexpr MVTermPower()
      : terms(MakeArray<TSVTermPower, dim>(TSVTermPower())) {}

  constexpr explicit MVTermPower(const std::array<TSVTermPower, dim>& _terms)
      : terms(_terms) {}

  constexpr bool IsConstant() const {
    for (auto& t : terms) {
      if (!t.IsConstant()) return false;
    }
    return true;
  }

  constexpr bool IsPolynomial() const {
    for (auto& t : terms) {
      if (!t.IsPolynomial()) return false;
    }
    return true;
  }

  constexpr bool IsIndexUnused(unsigned index) const {
    return terms[index].IsConstant();
  }

  constexpr TValue Get(const TPoint& p) const {
    TValue r = TValue(1);
    for (unsigned i = 0; i < dim; ++i) r *= terms[i].Get(p(i));
    return r;
  }

  constexpr bool IsFinite(const TPoint& p) const {
    for (unsigned i = 0; i < dim; ++i) {
      if (!IsFinite(p(i))) return false;
    }
    return true;
  }

  constexpr TSVTermPower& operator()(unsigned index) { return terms[index]; }

  constexpr const TSVTermPower& operator()(unsigned index) const {
    return terms[index];
  }

  constexpr bool operator<(const TSelf& r) const { return terms < r.terms; }

  constexpr bool operator==(const TSelf& r) const { return terms == r.terms; }

  constexpr bool IsMultiplicable(const TSelf& r) const {
    for (unsigned i = 0; i < dim; ++i) {
      if (!terms[i].IsMultiplicable(r(i))) return false;
    }
    return true;
  }

  TSelf& operator*=(const TSelf& r) {
    for (unsigned i = 0; i < dim; ++i) terms[i] *= r(i);
    return *this;
  }

  TSelf operator*(const TSelf& r) const {
    TSelf t(*this);
    t *= r;
    return t;
  }

  constexpr bool IsDivisible(const TSelf& r) const {
    for (unsigned i = 0; i < dim; ++i) {
      if (!terms[i].IsDivisible(r(i))) return false;
    }
    return true;
  }

  TSelf& operator/=(const TSelf& r) {
    for (unsigned i = 0; i < dim; ++i) terms[i] /= r(i);
    return *this;
  }

  TSelf operator/(const TSelf& r) const {
    TSelf t(*this);
    t /= r;
    return t;
  }

  std::string ToString(const std::string& variable_name) const {
    std::string s;
    bool first = true;
    for (unsigned i = 0; i < dim; ++i) {
      std::string sname = terms[i].ToString(variable_name + std::to_string(i));
      if (!sname.empty()) {
        s = s + (first ? "" : "*") + sname;
        first = false;
      }
    }
    return s;
  }
};

template <unsigned dim>
using DMVTermPower = MVTermPower<double, dim>;
}  // namespace ext_polynomial
}  // namespace calculus
