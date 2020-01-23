#pragma once

#include "common/calculus/ext_polynomial/factorized.h"
#include "common/calculus/ext_polynomial/mv_function.h"
#include "common/calculus/ext_polynomial/mv_make.h"
#include "common/calculus/ext_polynomial/mv_substitution_factorized.h"
#include "common/calculus/ext_polynomial/mv_substitution_function.h"
#include "common/calculus/ext_polynomial/mv_substitution_index.h"
#include "common/calculus/ext_polynomial/mv_substitution_value.h"

namespace calculus {
namespace ext_polynomial {
template <class TValue, unsigned dim>
inline MVFunction<TValue, dim> SubstitutionAuto(
    const MVFunction<TValue, dim>& f, unsigned index, const TValue& sv) {
  return SubstitutionValue(f, index, sv, true);
}

template <class TValue, unsigned dim>
inline MVFunction<TValue, dim> SubstitutionAuto(
    const MVFunction<TValue, dim>& f, unsigned index,
    const Factorized<TValue>& sf) {
  return SubstitutionFactorized(f, index, sf);
}

template <class TValue, unsigned dim>
inline MVFunction<TValue, dim> SubstitutionAuto(
    const MVFunction<TValue, dim>& f, unsigned index,
    const MVFunction<TValue, dim>& sf) {
  if (sf.IsConstant()) return SubstitutionAuto(f, index, sf.ToValue());
  if (sf == MakeXi<TValue, dim>(index)) return f;
  for (unsigned i = 0; i < dim; ++i) {
    if (sf == MakeXi<TValue, dim>(i)) return SubstitutionIndex(f, index, i);
  }
  return SubstitutionFunction(f, index, sf);
}
}  // namespace ext_polynomial
}  // namespace calculus
