#pragma once

#include "common/calculus/ext_polynomial/integration.h"
#include "common/calculus/ext_polynomial/mv_function.h"
#include "common/calculus/ext_polynomial/mv_substitution_function.h"
#include "common/calculus/ext_polynomial/mv_substitution_value.h"
#include "common/calculus/ext_polynomial/mv_view.h"

namespace calculus {
namespace ext_polynomial {
template <class TValue, unsigned dim>
inline MVFunction<TValue, dim> Integration(const MVFunction<TValue, dim>& f,
                                           unsigned index) {
  auto svf = SVView(f, index);
  auto svfi = Integration(svf);
  return MVView(svfi, index);
}

template <class TValue, unsigned dim>
inline MVFunction<TValue, dim> Integration(const MVFunction<TValue, dim>& f,
                                           unsigned index,
                                           const TValue& limit_a,
                                           const TValue& limit_b,
                                           bool skip_non_finite) {
  auto fnew = Integration(f, index);
  return SubstitutionValue(fnew, index, limit_b, skip_non_finite) -
         SubstitutionValue(fnew, index, limit_a, skip_non_finite);
}

template <class TValue, unsigned dim>
inline MVFunction<TValue, dim> Integration(
    const MVFunction<TValue, dim>& f, unsigned index,
    const MVFunction<TValue, dim>& limit_a,
    const MVFunction<TValue, dim>& limit_b) {
  auto fnew = Integration(f, index);
  return SubstitutionFunction(fnew, index, limit_b) -
         SubstitutionFunction(fnew, index, limit_a);
}
}  // namespace ext_polynomial
}  // namespace calculus
