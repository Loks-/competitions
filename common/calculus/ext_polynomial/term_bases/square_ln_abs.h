#pragma once

#include "common/calculus/ext_polynomial/term_bases/ln_abs.h"
#include "common/calculus/ext_polynomial/term_bases/type.h"
#include <cmath>
#include <memory>
#include <string>

namespace calculus {
namespace ext_polynomial {
namespace term_bases {
template <class TValue>
class SquareLnAbs : public LnAbs<TValue> {
 public:
  using TBase = TermBase<TValue>;

  SquareLnAbs() {}
  Type GetType() const override { return Type::SQUARE_LN_ABS; }

  TValue BaseGet(const TValue& x) const override {
    auto t = log(fabs(x));
    return t * t;
  }

  std::string ToString(const std::string& variable_name) const override {
    return "ln^2(abs(" + variable_name + "))";
  }
};

template <class TValue>
inline PTermBase<TValue> MakeSquareLnAbs() {
  return std::make_shared<SquareLnAbs<TValue>>();
}
}  // namespace term_bases
}  // namespace ext_polynomial
}  // namespace calculus
