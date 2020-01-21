#pragma once

#include "common/base.h"
#include "common/calculus/ext_polynomial/division_linear.h"
#include "common/calculus/ext_polynomial/io.h"
#include "common/calculus/ext_polynomial/mv_function.h"
#include "common/calculus/ext_polynomial/mv_unused_index.h"
#include "common/calculus/ext_polynomial/mv_view.h"
#include <iostream>

// Division f / (xi - c)
namespace calculus {
namespace ext_polynomial {
template <class TValue, unsigned dim>
std::pair<MVFunction<TValue, dim>, MVFunction<TValue, dim>> DivisionLinearFQR(
    const MVFunction<TValue, dim>& f, unsigned index,
    const MVFunction<TValue, dim>& fc) {
  assert(IsIndexUnused(fc, index));
  auto p = DivisionLinearQR(SVPolynomialView(f, index), fc);
  return {MVView(p.first, index), MVView(p.second, index)};
}

template <class TValue, unsigned dim>
std::pair<MVFunction<TValue, dim>, MVFunction<TValue, dim>> DivisionLinearVQR(
    const MVFunction<TValue, dim>& f, unsigned index, const TValue& vc) {
  return DivisionLinearVQR(f, index, MVFunction<TValue, dim>(vc));
}

template <class TValue, unsigned dim>
MVFunction<TValue, dim> DivisionLinearF(const MVFunction<TValue, dim>& f,
                                        unsigned index,
                                        const MVFunction<TValue, dim>& fc,
                                        bool force_division) {
  auto p = DivisionLinearFQR(f, index, fc);
  if (p.second.Empty()) return p.first;
  if (force_division) {
    std::cout << "Dropping remainder: " << p.second << std::endl;
    return p.first;
  } else {
    return {};
  }
}
template <class TValue, unsigned dim>
MVFunction<TValue, dim> DivisionLinearV(const MVFunction<TValue, dim>& f,
                                        unsigned index, const TValue& vc,
                                        bool force_division) {
  return DivisionLinearF(f, index, MVFunction<TValue, dim>(vc), force_division);
}
}  // namespace ext_polynomial
}  // namespace calculus
