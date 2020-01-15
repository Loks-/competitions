#pragma once

#include "common/calculus/ext_polynomial/multivariable/term.h"
#include "common/calculus/ext_polynomial/term_bases/one.h"
#include "common/stl/array.h"

namespace calculus {
namespace ext_polynomial {
namespace multivariable {
template <class TValue, unsigned dim>
inline TermBase<TValue, dim> MakeOne() {
  auto p = calculus::ext_polynomial::term_bases::MakeOne<TValue>(0);
  return MakeArray<PTermBase<TValue>, dim>(p);
}
}  // namespace multivariable
}  // namespace ext_polynomial
}  // namespace calculus
