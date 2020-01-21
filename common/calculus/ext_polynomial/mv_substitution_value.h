#pragma once

#include "common/base.h"
#include "common/calculus/ext_polynomial/division.h"
#include "common/calculus/ext_polynomial/multiplication.h"
#include "common/calculus/ext_polynomial/mv_function.h"
#include "common/calculus/ext_polynomial/mv_term.h"
#include "common/calculus/ext_polynomial/term_power.h"
#include "common/numeric/utils/pow.h"

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
    const MVFunction<TValue, dim>& f, unsigned index, const TValue& sv) {
  MVFunction<TValue, dim> fnew;
  for (auto& t : f.terms) fnew.AddTermsUnsafe(SubstitutionValue(t, index, sv));
  fnew.Compress();
  return fnew;
}
}  // namespace ext_polynomial
}  // namespace calculus
