#pragma once

#include "common/calculus/ext_polynomial/mv_function.h"
#include "common/calculus/ext_polynomial/mv_term.h"
#include "common/calculus/ext_polynomial/mv_view.h"
#include "common/calculus/ext_polynomial/substitution_shift.h"

// x_new = x_old - c
// x_old = x_new + c
namespace calculus {
namespace ext_polynomial {
template <class TValue, unsigned dim>
inline MVFunction<TValue, dim> SubstitutionShift(const MVTerm<TValue, dim>& t,
                                                 unsigned index,
                                                 const TValue& s) {
  auto svt = SVView(t, index);
  return MVView(SubstitutionShift(svt, s), index);
}

template <class TValue, unsigned dim>
inline MVFunction<TValue, dim> SubstitutionShift(
    const MVFunction<TValue, dim>& f, unsigned index, const TValue& s) {
  MVFunction<TValue, dim> fnew;
  for (auto& t : f.terms) fnew.AddTermsUnsafe(SubstitutionShift(t, index, s));
  fnew.Compress();
  return fnew;
}
}  // namespace ext_polynomial
}  // namespace calculus
