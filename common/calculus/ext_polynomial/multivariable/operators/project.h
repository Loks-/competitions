#pragma once

#include "common/calculus/ext_polynomial/multivariable/function.h"
#include "common/calculus/ext_polynomial/multivariable/term.h"
#include "common/calculus/ext_polynomial/multivariable/term_power.h"

namespace calculus {
namespace ext_polynomial {
namespace multivariable {
namespace operators {
template <class TValue, unsigned dim>
inline Term<TValue, dim> Project(const Term<TValue, dim>& t, unsigned index,
                                 const TValue& xi) {
  TermPower<TValue, dim> tp_new = t.tp;
  tp_new(index) = SVTermPower<TValue>();
  return {t.a * t.tp(index).Get(xi), tp_new};
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> Project(const Function<TValue, dim>& f,
                                     unsigned index, const TValue& xi) {
  Function<TValue, dim> fnew;
  for (auto& t : f.terms) fnew.AddTerm(Project(t, index, xi));
  fnew.Compress();
  return fnew;
}

}  // namespace operators
}  // namespace multivariable
}  // namespace ext_polynomial
}  // namespace calculus
