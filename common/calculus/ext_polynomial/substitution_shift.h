#pragma once

#include "common/base.h"
#include "common/calculus/ext_polynomial/function.h"
#include "common/calculus/ext_polynomial/make.h"
#include "common/calculus/ext_polynomial/multiplication.h"
#include "common/calculus/ext_polynomial/substitution_function.h"
#include "common/calculus/ext_polynomial/term.h"
#include "common/calculus/ext_polynomial/term_bases/ln_abs.h"
#include "common/calculus/ext_polynomial/term_bases/ln_abs_c.h"
#include "common/calculus/ext_polynomial/term_bases/one.h"
#include "common/calculus/ext_polynomial/term_bases/type.h"

// x_new = x_old - c
// x_old = x_new + c
namespace calculus {
namespace ext_polynomial {
template <class TValueF, class TValueTerm>
inline Function<TValueF, TValueTerm> SubstitutionShift(
    const Term<TValueF, TValueTerm>& t, const TValueTerm& s) {
  if (s == TValueTerm(0)) return t;
  auto tnew = t;
  auto tb = tnew.tp.base;
  tnew.tp.base = term_bases::MakeOne<TValueTerm>();
  auto sf = MakeLinear<TValueF, TValueTerm>(-s);
  auto fb = SubstitutionFunction(tnew, sf);
  switch (tb->GetType()) {
    case term_bases::Type::ONE:
      return fb;
    case term_bases::Type::LN_ABS:
      return fb * Term<TValueF, TValueTerm>(
                      TValueF(1), TermPower<TValueTerm>(
                                      term_bases::MakeLnAbsC<TValueTerm>(-s)));
    case term_bases::Type::LN_ABS_C: {
      auto p = dynamic_cast<const term_bases::LnAbsC<TValueTerm>*>(tb.get());
      assert(p);
      if (p->c != s)
        return fb * Term<TValueF, TValueTerm>(
                        TValueF(1),
                        TermPower<TValueTerm>(
                            term_bases::MakeLnAbsC<TValueTerm>(p->c - s)));
      else
        return fb * Term<TValueF, TValueTerm>(
                        TValueF(1), TermPower<TValueTerm>(
                                        term_bases::MakeLnAbs<TValueTerm>()));
    }
    default:
      assert(false);
  }
  return {};
}

template <class TValueF, class TValueTerm>
inline Function<TValueF, TValueTerm> SubstitutionShift(
    const Function<TValueF, TValueTerm>& f, const TValueTerm& s) {
  Function<TValueF, TValueTerm> fnew;
  for (auto& t : f.terms) fnew.AddTermsUnsafe(SubstitutionShift(t, s));
  fnew.Compress();
  return fnew;
}
}  // namespace ext_polynomial
}  // namespace calculus
