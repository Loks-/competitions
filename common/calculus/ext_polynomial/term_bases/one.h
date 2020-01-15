#pragma once

#include "common/calculus/ext_polynomial/term_base.h"
#include <memory>

namespace calculus {
namespace ext_polynomial {
namespace term_bases {
template <class TValue>
class One : public TermBase<TValue> {
 public:
  using TBase = TermBase<TValue>;

  One(int _power) : TermBase<TValue>(_power) {}

  TValue TermBaseGet(const TValue& x) const override { return TValue(1); }

  bool TermBaseLess(const TBase& r) const override {
    auto p = dynamic_cast<One*>(&r);
    return (p == nullptr);
  }

  bool TermBaseEqual(const TBase& r) const override {
    auto p = dynamic_cast<One*>(&r);
    return (p != nullptr);
  }
};

template <class TValue>
inline PTermBase<TValue> CreateOne(int power = 0) {
  return std::make_shared<One<TValue>>(power);
}
}  // namespace term_bases
}  // namespace ext_polynomial
}  // namespace calculus
