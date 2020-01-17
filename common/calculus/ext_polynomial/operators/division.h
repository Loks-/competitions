#pragma once

#include "common/assert_exception.h"
#include "common/calculus/ext_polynomial/term.h"
#include "common/calculus/ext_polynomial/term_base.h"
#include "common/calculus/ext_polynomial/term_power.h"

namespace calculus {
namespace ext_polynomial {
namespace operators {
template <class TValue>
inline bool IsSimpleDivision(const PTermBase<TValue>& p1,
                             const PTermBase<TValue>& p2) {
  return p2->IsOne();
}

template <class TValue>
inline PTermBase<TValue> SimpleDivision(const PTermBase<TValue>& p1,
                                        const PTermBase<TValue>& p2) {
  Assert(p2->IsOne());
  return p1;
}

template <class TValue>
inline bool IsSimpleDivision(const TermPower<TValue>& tp1,
                             const TermPower<TValue>& tp2) {
  return IsSimpleDivision(tp1.base, tp2.base);
}

template <class TValue>
inline TermPower<TValue> SimpleDivision(const TermPower<TValue>& tp1,
                                        const TermPower<TValue>& tp2) {
  return TermPower<TValue>(SimpleDivision(tp1.base, tp2.base),
                           tp1.power - tp2.power);
}

template <class TValue>
inline bool IsSimpleDivision(const Term<TValue>& t1, const Term<TValue>& t2) {
  return IsSimpleDivision(t1.tp, t2.tp);
}

template <class TValue>
inline Term<TValue> SimpleDivision(const Term<TValue>& t1,
                                   const Term<TValue>& t2) {
  return Term<TValue>(t1.a / t2.a, SimpleDivision(t1.tp, t2.tp));
}
}  // namespace operators
}  // namespace ext_polynomial
}  // namespace calculus
