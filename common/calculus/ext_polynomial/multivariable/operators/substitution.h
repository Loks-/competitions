#pragma once

#include "common/assert_exception.h"
#include "common/calculus/ext_polynomial/multivariable/function.h"
#include "common/calculus/ext_polynomial/multivariable/term.h"
#include "common/calculus/ext_polynomial/multivariable/term_power.h"
#include "common/calculus/ext_polynomial/operators/multiplication.h"
#include "common/calculus/ext_polynomial/term_bases/type.h"
#include "common/numeric/utils/pow.h"

namespace calculus {
namespace ext_polynomial {
namespace multivariable {
namespace operators {
template <class TValue, unsigned dim>
inline Function<TValue, dim> SubstitutionIndex(const Term<TValue, dim>& t,
                                               unsigned old_index,
                                               unsigned new_index) {
  SVTermPower<TValue> sv_term_i = t.tp(old_index);
  SVTermPower<TValue> sv_term_j = t.tp(new_index);
  Assert(calculus::ext_polynomial::operators::IsSimpleMultiplication(sv_term_i,
                                                                     sv_term_j),
         "Simple index substitution is impossible.");
  Term<TValue, dim> t_new = t;
  t_new.tp(new_index) =
      calculus::ext_polynomial::operators::SimpleMultiplication(sv_term_i,
                                                                sv_term_j);
  t_new.tp(old_index) = SVTermPower<TValue>();
  return t_new;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> SubstitutionIndex(const Function<TValue, dim>& f,
                                               unsigned old_index,
                                               unsigned new_index) {
  Function<TValue, dim> fnew;
  for (auto& t : f.terms) {
    auto ft = SubstitutionIndex(t, old_index, new_index);
    for (auto& tt : ft.terms) fnew.AddTerm(tt);
  }
  fnew.Compress();
  return fnew;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> Substitution(const Term<TValue, dim>& t,
                                          unsigned index,
                                          const Function<TValue, dim>& sf) {
  SVTermPower<TValue> sv_term_i = t.tp(index);
  Term<TValue, dim> t_new = t;
  t_new.tp(index) = SVTermPower<TValue>();
  Function<TValue, dim> f;
  switch (sv_term_i.GetType()) {
    case calculus::ext_polynomial::term_bases::Type::ONE:
      if (sv_term_i.power == 0) {
        return t_new;
      } else if (sv_term_i.power > 0) {
        f = PowU(sf, sv_term_i.power);
      } else {
        Assert(false, "Substitution with negative power is not supported.");
      }
      break;
    default:
      Assert(false,
             "Substitution with current base functions is not supported.");
  }
  return f * Function<TValue, dim>(t_new);
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> Substitution(const Function<TValue, dim>& f,
                                          unsigned index,
                                          const Function<TValue, dim>& sf) {
  Function<TValue, dim> fnew;
  for (auto& t : f.terms) {
    auto ft = Substitution(t, index, sf);
    for (auto& tt : ft.terms) fnew.AddTerm(tt);
  }
  fnew.Compress();
  return fnew;
}
}  // namespace operators
}  // namespace multivariable
}  // namespace ext_polynomial
}  // namespace calculus
