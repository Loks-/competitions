#pragma once

#include "common/base.h"
#include "common/calculus/ext_polynomial/function.h"
#include "common/calculus/ext_polynomial/term.h"

namespace calculus {
namespace ext_polynomial {
template <class TValue, class TTerm>
inline Function<TValue, TTerm> DivisionTerms(const TTerm& l, const TTerm& r) {
  if (l.IsDivisible(r)) {
    return l / r;
  }
  assert(false);
  return {};
}

template <class TValue, class TTerm>
inline Function<TValue, TTerm> operator/(const Function<TValue, TTerm>& f1,
                                         const TTerm& t2) {
  Function<TValue, TTerm> f;
  for (auto& t1 : f1.terms) {
    if (t1.IsDivisible(t2)) {
      f.AddTermUnsafe(t1 / t2);
    } else {
      f.AddTermsUnsafe(DivisionTerms(t1, t2));
    }
  }
  f.Compress();
  return f;
}

template <class TValue, class TTerm>
inline Function<TValue, TTerm> operator/(const Function<TValue, TTerm>& f1,
                                         const Function<TValue, TTerm>& f2) {
  assert(f2.Size() == 1);
  return f1 / f2(0);
}

template <class TValue, class TTerm>
inline Function<TValue, TTerm>& operator/=(Function<TValue, TTerm>& f1,
                                           const TTerm& t2) {
  return f1 = (f1 / t2);
}

template <class TValue, class TTerm>
inline Function<TValue, TTerm>& operator/=(Function<TValue, TTerm>& f1,
                                           const Function<TValue, TTerm>& f2) {
  return f1 = (f1 / f2);
}
}  // namespace ext_polynomial
}  // namespace calculus
