#pragma once

#include "common/calculus/ext_polynomial/multivariable/function.h"
#include "common/calculus/ext_polynomial/multivariable/operators/project.h"
#include "common/calculus/ext_polynomial/multivariable/operators/substitution.h"
#include "common/calculus/ext_polynomial/multivariable/term.h"
#include "common/calculus/ext_polynomial/multivariable/term_power.h"
#include "common/calculus/ext_polynomial/operators/intergration.h"

namespace calculus {
namespace ext_polynomial {
namespace multivariable {
namespace operators {
template <class TValue, unsigned dim>
inline Function<TValue, dim> Integration(const Term<TValue, dim>& t,
                                         unsigned index) {
  TermPower<TValue, dim> tp = t.tp;
  SVTermPower<TValue> sv_term_i = tp(index);
  auto sv_f = calculus::ext_polynomial::operators::Integration(
      SVTerm<TValue>(TValue(1), sv_term_i));
  Function<TValue, dim> f;
  for (auto& svt : sv_f.terms) {
    tp(index) = svt.tp;
    f.AddTerm({t.a * svt.a, tp});
  }
  return f;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> Integration(const Function<TValue, dim>& f,
                                         unsigned index) {
  Function<TValue, dim> fnew;
  for (auto& t : f.terms) {
    auto ft = Integration(t, index);
    for (auto& tt : ft.terms) fnew.AddTerm(tt);
  }
  fnew.Compress();
  return fnew;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> Integration(const Function<TValue, dim>& f,
                                         unsigned index, const TValue& a,
                                         const TValue& b) {
  auto fnew = Integration(f, index);
  return Project(fnew, index, b) - Project(fnew, index, a);
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> Integration(const Function<TValue, dim>& f,
                                         unsigned index,
                                         const Function<TValue, dim>& sa,
                                         const Function<TValue, dim>& sb) {
  auto fnew = Integration(f, index);
  return Substitution(fnew, index, sb) - Substitution(fnew, index, sa);
}
}  // namespace operators
}  // namespace multivariable
}  // namespace ext_polynomial
}  // namespace calculus
