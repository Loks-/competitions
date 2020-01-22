#pragma once

#include "common/calculus/ext_polynomial/function.h"
#include "common/calculus/ext_polynomial/substitution_shift.h"
#include <iostream>

namespace calculus {
namespace ext_polynomial {
template <class TValueF, class TValueTerm>
inline TValueF SubstitutionValue(const Function<TValueF, TValueTerm>& f,
                                 const TValueTerm& sv, bool skip_non_finite) {
  TValueF s(0);
  Function<TValueF, TValueTerm> f_non_finite;
  for (auto& t : f.terms) {
    if (t.IsFinite(sv)) {
      s += t.Get(sv);
    } else {
      f_non_finite.AddTermUnsafe(t);
    }
  }
  if (!f_non_finite.Empty()) {
    // Try to move to zero
    TValueTerm zero(0);
    auto fnf_s = SubstitutionShift(f_non_finite, sv);
    for (auto& t : fnf_s.terms) {
      if (t.IsFinite(zero)) {
        s += t.Get(zero);
      } else {
        if (skip_non_finite) {
          std::cout << "[Warning] Skipping non-finite term: " << t.ToString("x")
                    << std::endl;
        } else {
          s += t.Get(zero);
        }
      }
    }
  }
  return s;
}
}  // namespace ext_polynomial
}  // namespace calculus
