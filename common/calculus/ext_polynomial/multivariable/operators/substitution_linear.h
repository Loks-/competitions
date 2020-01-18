#pragma once

#include "common/assert_exception.h"
#include "common/calculus/ext_polynomial/multivariable/function.h"
#include "common/calculus/ext_polynomial/multivariable/linear.h"
#include "common/calculus/ext_polynomial/multivariable/operators.h"
#include "common/calculus/ext_polynomial/multivariable/operators/division_linear.h"
#include "common/calculus/ext_polynomial/multivariable/operators/linear.h"
#include "common/calculus/ext_polynomial/multivariable/operators/substitution.h"
#include "common/calculus/ext_polynomial/term_bases/ln_abs.h"
#include "common/calculus/ext_polynomial/term_bases/ln_abs_c.h"
#include "common/calculus/ext_polynomial/term_bases/one.h"
#include "common/calculus/ext_polynomial/term_bases/type.h"
#include "common/numeric/utils/pow.h"
#include <algorithm>
#include <cmath>

namespace calculus {
namespace ext_polynomial {
namespace multivariable {
namespace operators {
template <class TValue, unsigned dim>
inline Function<TValue, dim> SubstitutionLinear(const Function<TValue, dim>& f,
                                                unsigned index,
                                                const Factorized<TValue>& sf) {
  auto pone = calculus::ext_polynomial::term_bases::MakeOne<TValue>();

  // Replace ln and calc minp and maxp.
  Function<TValue, dim> f1;
  int minp = 0, maxp = 0;
  for (auto t : f.terms) {
    auto svtp = t.tp(index);
    int p = svtp.power;
    minp = std::min(minp, p);
    maxp = std::max(maxp, p);
    switch (svtp.GetType()) {
      case calculus::ext_polynomial::term_bases::Type::ONE:
        f1.AddTerm(t);
        break;
      case calculus::ext_polynomial::term_bases::Type::LN_ABS:
        t.tp(index) = pone;
        if (sf.a != TValue(1)) {
          TValue la = log(fabs(sf.a));
          f1.AddTerm({t.a * la, t.tp});
        }
        for (auto& l : sf.vn) {
          Assert(t.tp(l.index).IsPolynomial(),
                 "Only polynomial is supported for substituion.");
          if (l.c != TValue(0)) {
            t.tp(l.index) =
                calculus::ext_polynomial::term_bases::MakeLnAbsC<TValue>(l.c);
            f1.AddTerm(t);
            t.tp(l.index) = pone;
          } else {
            t.tp(l.index) =
                calculus::ext_polynomial::term_bases::MakeLnAbs<TValue>();
            f1.AddTerm(t);
            t.tp(l.index) = pone;
          }
        }
        for (auto& l : sf.vd) {
          Assert(t.tp(l.index).IsPolynomial(),
                 "Only polynomial is supported for substituion.");
          if (l.c != TValue(0)) {
            t.tp(l.index) =
                calculus::ext_polynomial::term_bases::MakeLnAbsC<TValue>(l.c);
            f1.AddTerm(-t);
            t.tp(l.index) = pone;
          } else {
            t.tp(l.index) =
                calculus::ext_polynomial::term_bases::MakeLnAbs<TValue>();
            f1.AddTerm(-t);
            t.tp(l.index) = pone;
          }
        }
        break;
      default:
        Assert(false, "Term is not supported in SubstitutionLinear.");
    }
  }
  f1.Compress();

  // Adjust power in all terms to avoid unsupported substitution
  Function<TValue, dim> f2;
  int adjusted_up = 0;
  if (sf.SimpleD()) {
    Function<TValue, dim> ft;
    if (minp < 0) {
      adjusted_up = -minp;
      for (auto t : f1.terms) {
        t.tp(index).power += adjusted_up;
        ft.AddTerm(t);
      }
    } else {
      ft = f1;
    }
    f2 = Substitution(ft, index, ToFunction<TValue, dim>(sf));
  } else {
    Assert(false, "Non simple denominator is not supported yet.");
  }

  if (adjusted_up) {
    Function<TValue, dim> fa(TValue(1) / sf.a);
    for (auto& l : sf.vd) fa *= ToFunction<TValue, dim>(l);
    f2 *= PowU(fa, adjusted_up);
    for (auto& l : sf.vn) {
      for (int i = 0; i < adjusted_up; ++i) f2 = DivisionLinear(f2, l);
    }
  }

  return f2;
}
}  // namespace operators
}  // namespace multivariable
}  // namespace ext_polynomial
}  // namespace calculus
