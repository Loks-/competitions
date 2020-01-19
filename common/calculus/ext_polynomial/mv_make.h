#pragma once

#include "common/base.h"
#include "common/calculus/ext_polynomial/mv_function.h"
#include "common/calculus/ext_polynomial/mv_term.h"
#include "common/calculus/ext_polynomial/mv_term_power.h"

namespace calculus {
namespace ext_polynomial {
template <class TValue, unsigned dim>
inline MVFunction<TValue, dim> MakeXi(unsigned i, const TValue& a = TValue(1)) {
  assert(i < dim);
  MVTermPower<TValue, dim> tp;
  tp(i).power = 1;
  return MVFunction<TValue, dim>(MVTerm<TValue, dim>(a, tp));
}

template <unsigned dim>
inline DMVFunction<dim> DMakeXi(unsigned i, double a = 1.) {
  return MakeXi<double, dim>(i, a);
}
}  // namespace ext_polynomial
}  // namespace calculus
