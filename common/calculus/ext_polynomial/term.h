#pragma once

#include "common/calculus/ext_polynomial/term_base.h"

namespace calculus {
namespace ext_polynomial {
template <class TValue>
class Term {
 public:
  using TTermBase = PTermBase<TValue>;

  TValue a;
  TTermBase b;

  Term(const TValue& _a, TTermBase _b) : a(_a), b(_b) {}

  TValue Get(const TValue& x) const { return a * b->Get(x); }
};
}  // namespace ext_polynomial
}  // namespace calculus
