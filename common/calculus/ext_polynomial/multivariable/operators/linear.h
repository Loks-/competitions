#pragma once

#include "common/assert_exception.h"
#include "common/calculus/ext_polynomial/multivariable/function.h"
#include "common/calculus/ext_polynomial/multivariable/linear.h"
#include "common/calculus/ext_polynomial/multivariable/make.h"
#include "common/calculus/ext_polynomial/multivariable/operators.h"

namespace calculus {
namespace ext_polynomial {
namespace multivariable {
namespace operators {
template <class TValue, unsigned dim>
Function<TValue, dim> ToFunction(const Linear<TValue>& l) {
  auto f = MakeXi<TValue, dim>(l.index);
  if (l.c != TValue(0)) {
    f -= MakeConstant<TValue, dim>(l.c);
  }
  return f;
}

template <class TValue, unsigned dim>
Function<TValue, dim> ToFunction(const Factorized<TValue>& f) {
  if (f.SimpleD()) {
    Function<TValue, dim> fnew = MakeConstant<TValue, dim>(f.a);
    for (auto& l : f.vn) fnew *= ToFunction<TValue, dim>(l);
    for (auto& l : f.vd) fnew /= ToFunction<TValue, dim>(l);
    return fnew;
  } else {
    Assert(false, "Non simple denominator is not supported yet.");
    return {};
  }
}

}  // namespace operators
}  // namespace multivariable
}  // namespace ext_polynomial
}  // namespace calculus
