#pragma once

#include "common/calculus/ext_polynomial/division.h"
#include "common/calculus/ext_polynomial/function.h"
#include "common/calculus/ext_polynomial/multiplication.h"
#include "common/calculus/ext_polynomial/mv_function.h"
#include "common/calculus/ext_polynomial/mv_term.h"
#include "common/calculus/ext_polynomial/term.h"
#include "common/calculus/ext_polynomial/term_power.h"

namespace calculus {
namespace ext_polynomial {
template <class TValue, unsigned dim>
inline Term<MVFunction<TValue, dim>, TValue> SVView(
    const MVTerm<TValue, dim>& t, unsigned index) {
  using TMVF = MVFunction<TValue, dim>;
  MVTerm<TValue, dim> tnew(t);
  tnew.tp(index) = TermPower<TValue>();
  TermPower<TValue> tp = t.tp(index);
  return Term<TMVF, TValue>(TMVF(tnew), tp);
}

template <class TValue, unsigned dim>
inline Function<MVFunction<TValue, dim>, TValue> SVView(
    const MVFunction<TValue, dim>& f, unsigned index) {
  Function<MVFunction<TValue, dim>, TValue> svf;
  for (auto& t : f.terms) svf.AddTermUnsafe(SVView(t, index));
  svf.Compress();
  return svf;
}

template <class TValue, unsigned dim>
inline MVFunction<TValue, dim> MVView(
    const Term<MVFunction<TValue, dim>, TValue>& t, unsigned index) {
  using TMVF = MVFunction<TValue, dim>;
  MVTermPower<TValue, dim> mtp;
  mtp(index) = t.tp;
  return t.a * MVTerm<TValue, dim>(TValue(1), mtp);
}

template <class TValue, unsigned dim>
inline MVFunction<TValue, dim> MVView(
    const Function<MVFunction<TValue, dim>, TValue>& f, unsigned index) {
  MVFunction<TValue, dim> fnew;
  for (auto& t : f.terms) fnew.AddTermsUnsafe(MVView(t, index));
  fnew.Compress();
  return fnew;
}
}  // namespace ext_polynomial
}  // namespace calculus
