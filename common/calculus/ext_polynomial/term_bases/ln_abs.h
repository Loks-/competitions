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

  LnAbs() {}
  Type GetType() const override { return Type::LN_ABS; }
  TValue Get(const TValue& x) const override { return log(fabs(x)); }

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
