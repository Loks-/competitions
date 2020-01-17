#pragma once

#include "common/calculus/ext_polynomial/multivariable/term.h"
#include "common/calculus/ext_polynomial/multivariable/term_power.h"
#include "common/calculus/ext_polynomial/operators/multiplication.h"

namespace calculus {
namespace ext_polynomial {
namespace multivariable {
namespace operators {
template <class TValue, unsigned dim>
inline bool IsSimpleMultiplication(const TermPower<TValue, dim>& tp1,
                                   const TermPower<TValue, dim>& tp2) {
  for (unsigned i = 0; i < dim; ++i) {
    if (!calculus::ext_polynomial::operators::IsSimpleMultiplication(tp1(i),
                                                                     tp2(i)))
      return false;
  }
  return true;
}

template <class TValue, unsigned dim>
inline TermPower<TValue, dim> SimpleMultiplication(
    const TermPower<TValue, dim>& tp1, const TermPower<TValue, dim>& tp2) {
  TermPower<TValue, dim> tp;
  for (unsigned i = 0; i < dim; ++i) {
    tp(i) = calculus::ext_polynomial::operators::SimpleMultiplication(tp1(i),
                                                                      tp2(i));
  }
  return tp;
}

template <class TValue, unsigned dim>
inline bool IsSimpleMultiplication(const Term<TValue, dim>& t1,
                                   const Term<TValue, dim>& t2) {
  return IsSimpleMultiplication(t1.tp, t2.tp);
}

template <class TValue, unsigned dim>
inline Term<TValue, dim> SimpleMultiplication(const Term<TValue, dim>& t1,
                                              const Term<TValue, dim>& t2) {
  return Term<TValue, dim>(t1.a * t2.a, SimpleMultiplication(t1.tp, t2.tp));
}
}  // namespace operators
}  // namespace multivariable
}  // namespace ext_polynomial
}  // namespace calculus
