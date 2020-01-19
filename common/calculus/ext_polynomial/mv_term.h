#pragma once

#include "common/calculus/ext_polynomial/mv_term_power.h"
#include "common/calculus/ext_polynomial/term.h"

namespace calculus {
namespace ext_polynomial {
template <class TValue, unsigned dim>
using MVTerm = Term<TValue, TValue, MVTermPower<TValue, dim>>;

template <unsigned dim>
using DMVTerm = MVTerm<double, dim>;
}  // namespace ext_polynomial
}  // namespace calculus
