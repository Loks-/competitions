#pragma once

#include "common/base.h"
#include "common/calculus/ext_polynomial/division.h"
#include "common/calculus/ext_polynomial/function.h"
#include "common/calculus/ext_polynomial/multiplication.h"
#include "common/calculus/ext_polynomial/term.h"
#include "common/calculus/ext_polynomial/term_bases/type.h"
#include "common/numeric/utils/pow.h"

namespace calculus {
namespace ext_polynomial {
template <class TValueF, class TValueTerm>
inline Function<TValueF, TValueTerm> SubstitutionFunction(
    const Term<TValueF, TValueTerm>& t,
    const Function<TValueF, TValueTerm>& sf) {
  using TFunction = Function<TValueF, TValueTerm>;
  switch (t.tp.GetType()) {
    case term_bases::Type::ONE:
      if (t.tp.power == 0) {
        return t;
      } else if (t.tp.power > 0) {
        return TFunction(t.a) * PowU(sf, t.tp.power);
      } else {
        return TFunction(t.a) / PowU(sf, -t.tp.power);
      }
    default:
      assert(false);
  }
  return {};
}

template <class TValueF, class TValueTerm>
inline Function<TValueF, TValueTerm> SubstitutionFunction(
    const Function<TValueF, TValueTerm>& f,
    const Function<TValueF, TValueTerm>& sf) {
  Function<TValueF, TValueTerm> fnew;
  for (auto& t : f.terms) fnew.AddTermsUnsafe(SubstitutionFunction(t, sf));
  fnew.Compress();
  return fnew;
}
}  // namespace ext_polynomial
}  // namespace calculus
