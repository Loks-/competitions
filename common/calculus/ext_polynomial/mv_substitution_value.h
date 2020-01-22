#pragma once

#include "common/base.h"
#include "common/calculus/ext_polynomial/division.h"
#include "common/calculus/ext_polynomial/multiplication.h"
#include "common/calculus/ext_polynomial/mv_function.h"
#include "common/calculus/ext_polynomial/mv_substitution_shift.h"
#include "common/calculus/ext_polynomial/mv_term.h"
#include "common/calculus/ext_polynomial/term_power.h"
#include "common/numeric/utils/pow.h"
#include <iostream>

namespace calculus {
namespace ext_polynomial {
template <class TValue, unsigned dim>
inline MVFunction<TValue, dim> SubstitutionValue(const MVTerm<TValue, dim>& t,
                                                 unsigned index,
                                                 const TValue& sv) {
  TermPower<TValue> sv_term_i = t.tp(index);
  MVTerm<TValue, dim> tnew = t;
  tnew.tp(index) = TermPower<TValue>();
  tnew.a *= sv_term_i.Get(sv);
  return tnew;
}

template <class TValue, unsigned dim>
inline MVFunction<TValue, dim> SubstitutionValue(
    const MVFunction<TValue, dim>& f, unsigned index, const TValue& sv,
    bool skip_non_finite) {
  MVFunction<TValue, dim> fnew, f_non_finite;
  for (auto& t : f.terms) {
    if (t.tp(index).IsFinite(sv)) {
      fnew.AddTermsUnsafe(SubstitutionValue(t, index, sv));
    } else {
      f_non_finite.AddTermUnsafe(t);
    }
  }
  if (!f_non_finite.Empty()) {
    // Try to move to zero
    TValue zero = TValue(0);
    auto fnf_s = SubstitutionShift(f_non_finite, index, sv);
    for (auto& t : fnf_s.terms) {
      if (t.tp(index).IsFinite(zero)) {
        fnew.AddTermsUnsafe(SubstitutionValue(t, index, zero));
      } else {
        if (skip_non_finite) {
          std::cout << "[Warning] Skipping non-finite term: " << t.ToString("x")
                    << std::endl;
        } else {
          fnew.AddTermsUnsafe(SubstitutionValue(t, index, zero));
        }
      }
    }
  }
  fnew.Compress();
  return fnew;
}
}  // namespace ext_polynomial
}  // namespace calculus
