#pragma once

#include "common/calculus/ext_polynomial/function.h"
#include "common/calculus/ext_polynomial/mv_term.h"

namespace calculus {
namespace ext_polynomial {
template <class TValue, unsigned dim>
using MVFunction = Function<TValue, MVTerm<TValue, dim>>;

template <unsigned dim>
using DMVFunction = MVFunction<double, dim>;
}  // namespace ext_polynomial
}  // namespace calculus
