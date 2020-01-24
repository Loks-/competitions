#pragma once

#include "common/base.h"
#include "common/calculus/ext_polynomial/function.h"
#include "common/calculus/ext_polynomial/substitution_shift.h"
#include "common/calculus/ext_polynomial/substitution_value.h"
#include "common/calculus/ext_polynomial/term.h"
#include "common/calculus/ext_polynomial/term_bases/ln_abs.h"
#include "common/calculus/ext_polynomial/term_bases/ln_abs_c.h"
#include "common/calculus/ext_polynomial/term_bases/one.h"
#include "common/calculus/ext_polynomial/term_bases/square_ln_abs.h"
#include "common/calculus/ext_polynomial/term_bases/type.h"

namespace calculus {
namespace ext_polynomial {
template <class TValueF, class TValueTerm>
inline Function<TValueF, TValueTerm> Integration(
    const Function<TValueF, TValueTerm>& f);

template <class TValueF, class TValueTerm>
inline Function<TValueF, TValueTerm> Integration(
    const Term<TValueF, TValueTerm>& t) {
  Function<TValueF, TValueTerm> output;
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
    case term_bases::Type::LN_ABS:
      if (tp.power != -1) {
        auto t_new = t;
        t_new.tp.power += 1;
        t_new.a /= t_new.tp.power;
        output.AddTermsUnsafe(t_new);
        t_new.tp.base = term_bases::MakeOne<TValueTerm>();
        t_new.a /= (-t_new.tp.power);
        output.AddTermsUnsafe(t_new);
      } else {
        std::cout << "." << std::endl;
        return Term<TValueF, TValueTerm>(
            t.a / 2, term_bases::MakeSquareLnAbs<TValueTerm>());
      }
      break;
    case term_bases::Type::LN_ABS_C: {
      auto p =
          dynamic_cast<const term_bases::LnAbsC<TValueTerm>*>(tp.base.get());
      auto f = SubstitutionShift(t, p->c);
      auto i = Integration(f);
      return SubstitutionShift(i, -p->c);
    }
    default:
      assert(false);
  }
  output.Compress();
  return output;
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
                           const TValueTerm& limit_a, const TValueTerm& limit_b,
                           bool skip_non_finite) {
  auto fi = Integration(f);
  return SubstitutionValue(fi, limit_b, skip_non_finite) -
         SubstitutionValue(fi, limit_a, skip_non_finite);
}
}  // namespace ext_polynomial
}  // namespace calculus
