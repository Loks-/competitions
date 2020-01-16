#pragma once

#include "common/calculus/ext_polynomial/term_power.h"

namespace calculus {
namespace ext_polynomial {
template <class TValue>
class Term {
 public:
  using TTermPower = TermPower<TValue>;

  TValue a;
  TTermPower tp;

  Term(const TValue& _a) : a(_a) {}
  Term(const TValue& _a, TTermPower _tp) : a(_a), tp(_tp) {}

  TValue Get(const TValue& x) const { return a * tp.Get(x); }
};
}  // namespace ext_polynomial
}  // namespace calculus
