#pragma once

#include "common/calculus/ext_polynomial/integration.h"
#include "common/calculus/ext_polynomial/mv_function.h"
#include "common/calculus/ext_polynomial/mv_substitution_function.h"
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
                                           const TValue& limit_b) {
  auto svf = SVView(f, index);
  return Integration(svf, limit_a, limit_b);
}

template <class TValue, unsigned dim>
inline MVFunction<TValue, dim> Integration(
    const MVFunction<TValue, dim>& f, unsigned index,
    const MVFunction<TValue, dim>& limit_a,
    const MVFunction<TValue, dim>& limit_b) {
  auto fnew = Integration(f, index);
  return Substitution(fnew, index, limit_b) -
         Substitution(fnew, index, limit_a);
}
}  // namespace ext_polynomial
}  // namespace calculus
