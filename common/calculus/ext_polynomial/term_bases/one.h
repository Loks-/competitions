#pragma once

#include "common/calculus/ext_polynomial/term_base.h"

namespace calculus {
namespace ext_polynomial {
namespace term_bases {
template <class TValue>
class One : public TermBase<TValue> {
 public:
  One(int _power) : TermBase<TValue>(_power) {}

  TValue GetTermBase(const TValue& x) const override { return TValue(1); }
};

template <class TValue>
inline PTermBase<TValue> CreateOne(int power = 0) {
  return std::make_shared<One<TValue>>(power);
}
}  // namespace term_bases
}  // namespace ext_polynomial
}  // namespace calculus
