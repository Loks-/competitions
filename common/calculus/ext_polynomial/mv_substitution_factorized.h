#pragma once

#include "common/base.h"
#include "common/calculus/ext_polynomial/factorized.h"
#include "common/calculus/ext_polynomial/factorized_conversion.h"
#include "common/calculus/ext_polynomial/factorized_division.h"
#include "common/calculus/ext_polynomial/factorized_function.h"
#include "common/calculus/ext_polynomial/mv_function.h"
#include "common/calculus/ext_polynomial/mv_substitution_function.h"
#include "common/calculus/ext_polynomial/term_bases/ln_abs.h"
#include "common/calculus/ext_polynomial/term_bases/ln_abs_c.h"
#include "common/calculus/ext_polynomial/term_bases/one.h"
#include "common/calculus/ext_polynomial/term_bases/type.h"
#include <algorithm>

namespace calculus {
namespace ext_polynomial {
template <class TValue, unsigned dim>
inline MVFunction<TValue, dim> SubstitutionFactorized(
    const MVFunction<TValue, dim>& f, unsigned index,
    const Factorized<TValue>& sf) {
  auto pone = term_bases::MakeOne<TValue>();

  // Replace ln and calc minp and maxp.
  MVFunction<TValue, dim> f1;
  int minp = 0, maxp = 0;
  for (auto t : f.terms) {
    auto svtp = t.tp(index);
    int p = svtp.power;
    minp = std::min(minp, p);
    maxp = std::max(maxp, p);
    switch (svtp.GetType()) {
      case term_bases::Type::ONE:
        f1.AddTermUnsafe(t);
        break;
      case term_bases::Type::LN_ABS:
        t.tp(index) = pone;
        if (sf.a != TValue(1)) {
          TValue la = log(fabs(sf.a));
          f1.AddTermUnsafe({t.a * la, t.tp});
        }
        for (auto& l : sf.vn) {
          assert(t.tp(l.index).IsPolynomial());
          if (l.c != TValue(0)) {
            t.tp(l.index) = term_bases::MakeLnAbsC<TValue>(l.c);
            f1.AddTermUnsafe(t);
            t.tp(l.index) = pone;
          } else {
            t.tp(l.index) = term_bases::MakeLnAbs<TValue>();
            f1.AddTermUnsafe(t);
            t.tp(l.index) = pone;
          }
        }
        for (auto& l : sf.vd) {
          assert(t.tp(l.index).IsPolynomial());
          if (l.c != TValue(0)) {
            t.tp(l.index) = term_bases::MakeLnAbsC<TValue>(l.c);
            f1.AddTermUnsafe(-t);
            t.tp(l.index) = pone;
          } else {
            t.tp(l.index) = term_bases::MakeLnAbs<TValue>();
            f1.AddTermUnsafe(-t);
            t.tp(l.index) = pone;
          }
        }
        break;
      default:
        assert(false);
    }
  }
  f1.Compress();

  // Adjust power in all terms to avoid unsupported substitution
  MVFunction<TValue, dim> f2;
  int adjusted_up = 0;
  if (sf.SimpleD()) {
    MVFunction<TValue, dim> ft;
    if (minp < 0) {
      adjusted_up = -minp;
      for (auto t : f1.terms) {
        t.tp(index).power += adjusted_up;
        ft.AddTermUnsafe(t);
      }
    } else {
      ft = f1;
    }
    f2 = SubstitutionFunction(ft, index, ToFunction<TValue, dim>(sf));
  } else {
    assert(false);
  }

  if (adjusted_up) {
    MVFunction<TValue, dim> fa(TValue(1) / sf.a);
    for (auto& l : sf.vd) fa *= ToFunction<TValue, dim>(l);
    f2 *= PowU(fa, adjusted_up);
    for (auto& l : sf.vn) {
      for (int i = 0; i < adjusted_up; ++i) f2 = f2 / l;
    }
  }

  return f2;
}
}  // namespace ext_polynomial
}  // namespace calculus
