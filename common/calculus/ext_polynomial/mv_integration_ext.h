#pragma once

#include "common/calculus/ext_polynomial/function.h"
#include "common/calculus/ext_polynomial/mv_integration.h"
#include "common/calculus/ext_polynomial/mv_substitution_auto.h"

namespace calculus {
namespace ext_polynomial {
template <class TValue, unsigned dim, class TValueA, class TValueB>
inline MVFunction<TValue, dim> IntegrationAB(const MVFunction<TValue, dim>& f,
                                             unsigned index,
                                             const TValueA& limit_a,
                                             const TValueB& limit_b) {
  auto fi = Integration(f, index);
  return SubstitutionAuto(fi, index, limit_b) -
         SubstitutionAuto(fi, index, limit_a);
}
}  // namespace ext_polynomial
}  // namespace calculus
