#pragma once

#include "common/base.h"
#include "common/calculus/ext_polynomial/term_base.h"
#include "common/calculus/ext_polynomial/term_bases/type.h"

#include <cmath>
#include <memory>
#include <string>

namespace calculus {
namespace ext_polynomial {
namespace term_bases {
template <class TValue>
class LnAbsC : public TermBase<TValue> {
 public:
  using TBase = TermBase<TValue>;

 public:
  TValue c;

 public:
  constexpr explicit LnAbsC(const TValue& _c) : c(_c) {}

  constexpr Type GetType() const override { return Type::LN_ABS_C; }

  constexpr TValue BaseGet(const TValue& x) const override {
    return log(fabs(x - c));
  }

  constexpr bool IsBaseFinite(const TValue& x) const override {
    return (x != c);
  }

  constexpr bool SameTypeLess(const TBase& r) const override {
    auto p = dynamic_cast<const LnAbsC*>(&r);
    assert(p);
    return c < p->c;
  }

  constexpr bool SameTypeEqual(const TBase& r) const override {
    auto p = dynamic_cast<const LnAbsC*>(&r);
    assert(p);
    return c == p->c;
  }

  std::string ToString(const std::string& variable_name) const override {
    return "ln(abs(" + variable_name + "-" + std::to_string(c) + "))";
  }
};

template <class TValue>
inline PTermBase<TValue> MakeLnAbsC(const TValue& c) {
  return std::make_shared<LnAbsC<TValue>>(c);
}
}  // namespace term_bases
}  // namespace ext_polynomial
}  // namespace calculus
