#pragma once

#include "common/assert_exception.h"
#include "common/calculus/ext_polynomial/term.h"
#include "common/calculus/ext_polynomial/term_base.h"
#include "common/calculus/ext_polynomial/term_power.h"

namespace calculus {
namespace ext_polynomial {
namespace operators {
template <class TValue>
inline bool IsSimpleProduct(const PTermBase<TValue>& p1,
                            const PTermBase<TValue>& p2) {
  return p1->IsOne() || p2->IsOne();
}

template <class TValue>
inline PTermBase<TValue> SimpleProduct(const PTermBase<TValue>& p1,
                                       const PTermBase<TValue>& p2) {
  if (p1->IsOne()) return p2;
  if (p2->IsOne()) return p1;
  Assert(false);
  return {};
}

template <class TValue>
inline bool IsSimpleProduct(const TermPower<TValue>& tp1,
                            const TermPower<TValue>& tp2) {
  return IsSimpleProduct(tp1.base, tp2.base);
}

template <class TValue>
inline TermPower<TValue> SimpleProduct(const TermPower<TValue>& tp1,
                                       const TermPower<TValue>& tp2) {
  return TermPower<TValue>(SimpleProduct(tp1.base, tp2.base),
                           tp1.power + tp2.power);
}

template <class TValue>
inline bool IsSimpleProduct(const Term<TValue>& t1, const Term<TValue>& t2) {
  return IsSimpleProduct(t1.tp, t2.tp);
}

template <class TValue>
inline Term<TValue> SimpleProduct(const Term<TValue>& t1,
                                  const Term<TValue>& t2) {
  return Term<TValue>(t1.a * t2.a, SimpleProduct(t1.tp, t2.tp));
}
}  // namespace operators
}  // namespace ext_polynomial
}  // namespace calculus
