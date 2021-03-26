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
  TValue BaseGet(const TValue&) const override { return TValue(1); }

  std::string ToString(const std::string&) const override { return ""; }
};

template <class TValue>
inline PTermBase<TValue> MakeOne() {
  return std::make_shared<One<TValue>>();
}
}  // namespace term_bases
}  // namespace ext_polynomial
}  // namespace calculus
