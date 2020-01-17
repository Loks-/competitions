#pragma once

#include "common/base.h"
#include "common/calculus/ext_polynomial/multivariable/function.h"
#include "common/calculus/ext_polynomial/multivariable/term.h"
#include "common/calculus/ext_polynomial/multivariable/term_power.h"

namespace calculus {
namespace ext_polynomial {
namespace multivariable {
template <class TValue, unsigned dim>
inline Function<TValue, dim> MakeConstant(TValue a = TValue(0)) {
  return Function<TValue, dim>(a);
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> MakeXi(unsigned i, TValue a = TValue(1)) {
  assert(i < dim);
  TermPower<TValue, dim> tp;
  tp(i).power = 1;
  return Function<TValue, dim>(Term<TValue, dim>(a, tp));
}

template <unsigned dim>
inline DFunction<dim> DMakeXi(unsigned i, double a = 1) {
  return MakeXi<double, dim>(i, a);
}
}  // namespace multivariable
}  // namespace ext_polynomial
}  // namespace calculus
