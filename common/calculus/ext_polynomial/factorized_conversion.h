#pragma once

#include "common/base.h"
#include "common/calculus/ext_polynomial/factorized.h"
#include "common/calculus/ext_polynomial/mv_function.h"
#include "common/calculus/ext_polynomial/mv_make.h"
#include <vector>

namespace calculus {
namespace ext_polynomial {
template <class TValue, unsigned dim>
MVFunction<TValue, dim> ToFunction(const FactorizedLinear<TValue>& l) {
  auto f = MakeXi<TValue, dim>(l.index);
  if (l.c != TValue(0)) f -= MVFunction<TValue, dim>(l.c);
  return f;
}

template <class TValue, unsigned dim>
MVFunction<TValue, dim> ToFunction(const Factorized<TValue>& f) {
  assert(f.SimpleD());
  MVFunction<TValue, dim> fnew(f.a);
  for (auto& l : f.vn) fnew *= ToFunction<TValue, dim>(l);
  for (auto& l : f.vd) fnew /= ToFunction<TValue, dim>(l);
  return fnew;
}

template <class TValue, unsigned dim>
MVFunction<TValue, dim> ToFunction(
    const std::vector<FactorizedLinear<TValue>>& vl) {
  return ToFunction<TValue, dim>(Factorized<TValue>(TValue(1), vl));
}
}  // namespace ext_polynomial
}  // namespace calculus
