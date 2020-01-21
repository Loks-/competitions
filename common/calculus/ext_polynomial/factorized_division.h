#pragma once

#include "common/base.h"
#include "common/calculus/ext_polynomial/division.h"
#include "common/calculus/ext_polynomial/factorized.h"
#include "common/calculus/ext_polynomial/factorized_conversion.h"
#include "common/calculus/ext_polynomial/io.h"
#include "common/calculus/ext_polynomial/mv_function.h"
#include "common/calculus/ext_polynomial/mv_view.h"
#include <iostream>
#include <utility>

namespace calculus {
namespace ext_polynomial {
template <class TValue, unsigned dim>
std::pair<MVFunction<TValue, dim>, MVFunction<TValue, dim>> DivisionQR(
    const MVFunction<TValue, dim>& f, const FactorizedLinear<TValue>& l) {
  MVFunction<TValue, dim> q;
  MVTerm<TValue, dim> qt(TValue(1));
  auto pvf = SVPolynomialView(f, l.index);
  auto pvl = SVPolynomialView(ToFunction<TValue, dim>(l), l.index);
  while (pvf.Size() > 1) {
    auto pvt = pvf.terms.back();
    pvt.tp.power -= 1;
    qt.tp(l.index).power = pvt.tp.power;
    q += pvt.a * qt;
    pvf -= pvt * pvl;
  }
  return {q, MVView(pvf, l.index)};
}

template <class TValue, unsigned dim>
MVFunction<TValue, dim> Division(const MVFunction<TValue, dim>& f,
                                 const FactorizedLinear<TValue>& l,
                                 bool force_division) {
  if (l.c == TValue(0)) return f / ToFunction<TValue, dim>(l);
  auto p = DivisionQR(f, l);
  if (p.second == TValue(0)) return p.first;
  if (force_division) {
    std::cout << "Dropping remainder: " << p.second << std::endl;
    return p.first;
  } else {
    return {};
  }
}

template <class TValue, unsigned dim>
MVFunction<TValue, dim> operator/(const MVFunction<TValue, dim>& f,
                                  const FactorizedLinear<TValue>& l) {
  return Division(f, l, true);
}
}  // namespace ext_polynomial
}  // namespace calculus
