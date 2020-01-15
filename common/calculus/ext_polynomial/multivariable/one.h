#pragma once

#include "common/calculus/ext_polynomial/multivariable/term.h"
#include "common/calculus/ext_polynomial/term_bases/one.h"

namespace calculus {
namespace ext_polynomial {
namespace multivariable {
template <class TValue, unsigned dim>
inline TermBase<TValue, dim> CreateOne() {
  auto p = calculus::ext_polynomial::term_bases::CreateOne<TValue>(0);
  return TermBase<TValue, dim>(dim, p);
}
}  // namespace multivariable
}  // namespace ext_polynomial
}  // namespace calculus
