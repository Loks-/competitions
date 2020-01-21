#pragma once

#include "common/calculus/ext_polynomial/mv_function.h"

namespace calculus {
namespace ext_polynomial {
template <class TValue, unsigned dim>
inline MVFunction<TValue, dim> SubstitutionIndex(const MVTerm<TValue, dim>& t,
                                                 unsigned old_index,
                                                 unsigned new_index) {
  if (old_index == new_index) return t;
  auto sv_term_i = t.tp(old_index);
  auto sv_term_j = t.tp(new_index);
  auto tnew = t;
  if (sv_term_i.IsMultiplicable(sv_term_j)) {
    tnew.tp(old_index) = TermPower<TValue>();
    tnew.tp(new_index) = sv_term_i * sv_term_j;
    return tnew;
  } else {
    assert(false);
    return {};
  }
}

template <class TValue, unsigned dim>
inline MVFunction<TValue, dim> SubstitutionIndex(
    const MVFunction<TValue, dim>& f, unsigned old_index, unsigned new_index) {
  MVFunction<TValue, dim> fnew;
  for (auto& t : f.terms)
    fnew.AddTermsUnsafe(SubstitutionIndex(t, old_index, new_index));
  fnew.Compress();
  return fnew;
}
}  // namespace ext_polynomial
}  // namespace calculus
