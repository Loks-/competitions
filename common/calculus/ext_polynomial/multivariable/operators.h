#pragma once

#include "common/calculus/ext_polynomial/multivariable/function.h"
#include "common/calculus/ext_polynomial/multivariable/term.h"

namespace calculus {
namespace ext_polynomial {
namespace multivariable {
template <class TValue, unsigned dim1, unsigned dim2>
inline Function<TValue, dim1 + dim2> CartesianProduct(
    const Function<TValue, dim1>& f1, const Function<TValue, dim2>& f2) {
  Function<TValue, dim1 + dim2> f;
  TermBase<TValue, dim1 + dim2> t;
  for (auto& t1 : f1.v) {
    for (unsigned i = 0; i < dim1; ++i) t[i] = t1.b[i];
    for (auto& t2 : f2.v) {
      for (unsigned i = 0; i < dim2; ++i) t[i + dim1] = t2.b[i];
      f.AddTerm({t1.a * t2.a, t});
    }
  }
  return f;
}
}  // namespace multivariable
}  // namespace ext_polynomial
}  // namespace calculus
