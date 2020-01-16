#pragma once

#include "common/calculus/ext_polynomial/term_base.h"
#include "common/calculus/ext_polynomial/term_bases/type.h"
#include <memory>
#include <string>

namespace calculus {
namespace ext_polynomial {
namespace term_bases {
template <class TValue>
class One : public TermBase<TValue> {
 public:
  using TBase = TermBase<TValue>;

  One() {}
  Type GetType() const override { return Type::ONE; }
  TValue Get(const TValue& x) const override { return TValue(1); }

  bool operator<(const TBase& r) const override { return !r.IsOne(); }

  bool operator==(const TBase& r) const override {
    return r.IsOne();
    // auto p = dynamic_cast<const One*>(&r);
    // return (p != nullptr);
  }

  std::string ToString(const std::string& variable_name) const override {
    return "";
  }
};

template <class TValue>
inline PTermBase<TValue> MakeOne() {
  return std::make_shared<One<TValue>>();
}
}  // namespace term_bases
}  // namespace ext_polynomial
}  // namespace calculus
