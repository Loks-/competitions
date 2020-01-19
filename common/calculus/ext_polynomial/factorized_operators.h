#pragma once

#include "common/base.h"
#include "common/calculus/ext_polynomial/factorized.h"
#include "common/calculus/ext_polynomial/mv_function.h"

namespace calculus {
namespace ext_polynomial {
template <class TValue, unsigned dim>
MVFunction<TValue, dim> operator/(const MVFunction<TValue, dim>& f,
                                  const FactorizedLinear<TValue>& l) {
  // ...
  return f;
}
}  // namespace ext_polynomial
}  // namespace calculus
