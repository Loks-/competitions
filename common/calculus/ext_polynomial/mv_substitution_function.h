#pragma once

#include "common/base.h"
#include "common/calculus/ext_polynomial/division.h"
#include "common/calculus/ext_polynomial/multiplication.h"
#include "common/calculus/ext_polynomial/mv_function.h"
#include "common/calculus/ext_polynomial/mv_term.h"
#include "common/calculus/ext_polynomial/term_bases/type.h"
#include "common/calculus/ext_polynomial/term_power.h"
#include "common/numeric/utils/pow.h"

namespace calculus {
namespace ext_polynomial {
template <class TValue, unsigned dim>
inline MVFunction<TValue, dim> SubstitutionFunction(
    const MVTerm<TValue, dim>& t, unsigned index,
    const MVFunction<TValue, dim>& sf) {
  TermPower<TValue> sv_term_i = t.tp(index);
  MVTerm<TValue, dim> tnew = t;
  tnew.tp(index) = TermPower<TValue>();
  MVFunction<TValue, dim> f;
  switch (sv_term_i.GetType()) {
    case term_bases::Type::ONE:
      if (sv_term_i.power == 0) {
        return tnew;
      } else if (sv_term_i.power > 0) {
        return MVFunction<TValue, dim>(tnew) * PowU(sf, sv_term_i.power);
      } else {
        return MVFunction<TValue, dim>(tnew) / PowU(sf, -sv_term_i.power);
      }
    default:
      assert(false);
  }
  return {};
}

template <class TValue, unsigned dim>
inline MVFunction<TValue, dim> SubstitutionFunction(
    const MVFunction<TValue, dim>& f, unsigned index,
    const MVFunction<TValue, dim>& sf) {
  MVFunction<TValue, dim> fnew;
  for (auto& t : f.terms)
    fnew.AddTermsUnsafe(SubstitutionFunction(t, index, sf));
  fnew.Compress();
  return fnew;
}
}  // namespace ext_polynomial
}  // namespace calculus
