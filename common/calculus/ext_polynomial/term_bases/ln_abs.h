#pragma once

#include "common/calculus/ext_polynomial/term_base.h"
#include "common/calculus/ext_polynomial/term_bases/type.h"

#include <cmath>
#include <memory>
#include <string>

namespace calculus {
namespace ext_polynomial {
namespace term_bases {
template <class TValue>
class LnAbs : public TermBase<TValue> {
 public:
  using TBase = TermBase<TValue>;

 public:
  constexpr LnAbs() {}

  constexpr Type GetType() const override { return Type::LN_ABS; }

  constexpr TValue BaseGet(const TValue& x) const override {
    return log(fabs(x));
  }

  constexpr TValue Get(const TValue& x, int power) const override {
    if (x == TValue(0) && (power > 0)) return TValue(0);
    return TBase::GetXPower(x, power) * BaseGet(x);
  }

  constexpr bool IsBaseFinite(const TValue& x) const override {
    return x != TValue(0);
  }

  constexpr bool IsFinite(const TValue& x, int power) const override {
    return (x != TValue(0)) || (power > 0);
  }

  std::string ToString(const std::string& variable_name) const override {
    return "ln(abs(" + variable_name + "))";
  }
};

template <class TValue>
inline PTermBase<TValue> MakeLnAbs() {
  return std::make_shared<LnAbs<TValue>>();
}
}  // namespace term_bases
}  // namespace ext_polynomial
}  // namespace calculus
