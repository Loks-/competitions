#pragma once

#include "common/calculus/ext_polynomial/mv_function.h"

namespace calculus {
namespace ext_polynomial {
template <class TValue, unsigned dim>
bool IsIndexUnused(const MVFunction<TValue, dim>& f, unsigned index) {
  for (auto& t : f.terms) {
    if (!t.tp.IsIndexUnused(index)) return false;
  }
  return true;
}
}  // namespace ext_polynomial
}  // namespace calculus
