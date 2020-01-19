#pragma once

#include "common/base.h"
#include "common/calculus/ext_polynomial/function.h"
#include "common/calculus/ext_polynomial/term.h"
#include "common/calculus/ext_polynomial/term_bases/ln_abs.h"
#include "common/calculus/ext_polynomial/term_bases/type.h"

namespace calculus {
namespace ext_polynomial {
template <class TValueF, class TValueTerm>
inline Function<TValueF, TValueTerm> Integration(
    const Term<TValueF, TValueTerm>& t) {
  const auto& tp = t.tp;
  switch (tp.GetType()) {
    case term_bases::Type::ONE:
      if (tp.power != -1) {
        auto tp_new = tp;
        tp_new.power += 1;
        return Term<TValueF, TValueTerm>(t.a / tp_new.power, tp_new);
      } else {
        return Term<TValueF, TValueTerm>(t.a,
                                         term_bases::MakeLnAbs<TValueTerm>());
      }
    default:
      assert(false);
      return {};
  }
}

template <class TValueF, class TValueTerm>
inline TValueF Integration(const Term<TValueF, TValueTerm>& t,
                           const TValueTerm& limit_a,
                           const TValueTerm& limit_b) {
  auto f = Integration(t);
  return f.Get(limit_b) - f.Get(limit_a);
}

template <class TValueF, class TValueTerm>
inline Function<TValueF, TValueTerm> Integration(
    const Function<TValueF, TValueTerm>& f) {
  Function<TValueF, TValueTerm> fnew;
  for (auto& t : f.terms) fnew.AddTermsUnsafe(Integration(t));
  fnew.Compress();
  return fnew;
}

template <class TValueF, class TValueTerm>
inline TValueF Integration(const Function<TValueF, TValueTerm>& f,
                           const TValueTerm& limit_a,
                           const TValueTerm& limit_b) {
  TValueF s(0);
  for (auto& t : f.terms) s += Integration(t, limit_a, limit_b);
  return s;
}
}  // namespace ext_polynomial
}  // namespace calculus
