#pragma once

#include "common/base.h"
#include "common/calculus/ext_polynomial/factorized.h"
#include "common/calculus/ext_polynomial/factorized_conversion.h"
#include "common/calculus/ext_polynomial/factorized_function.h"
#include "common/calculus/ext_polynomial/mv_function.h"
#include "common/calculus/ext_polynomial/mv_substitution_value.h"
#include "common/calculus/ext_polynomial/term_bases/ln_abs.h"
#include "common/calculus/ext_polynomial/term_bases/ln_abs_c.h"
#include "common/calculus/ext_polynomial/term_bases/one.h"
#include "common/calculus/ext_polynomial/term_bases/type.h"
#include "common/numeric/utils/pow.h"
#include <algorithm>

namespace calculus {
namespace ext_polynomial {
template <class TValue, unsigned dim>
inline MVFunction<TValue, dim> SubstitutionFactorized(
    const MVFunction<TValue, dim>& f, unsigned index,
    const Factorized<TValue>& sf) {
  if (sf.a == TValue(0)) return SubstitutionValue(f, index, sf.a);
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
        t.tp(index).base = pone;
        if (sf.a != TValue(1)) {
          TValue la = log(fabs(sf.a));
          f1.AddTermUnsafe({t.a * la, t.tp});
        }
        for (auto& l : sf.vn) {
          assert(t.tp(l.index).IsPolynomial());
          if (l.c != TValue(0)) {
            t.tp(l.index).base = term_bases::MakeLnAbsC<TValue>(l.c);
            f1.AddTermUnsafe(t);
            t.tp(l.index).base = pone;
          } else {
            t.tp(l.index).base = term_bases::MakeLnAbs<TValue>();
            f1.AddTermUnsafe(t);
            t.tp(l.index).base = pone;
          }
        }
        for (auto& l : sf.vd) {
          assert(t.tp(l.index).IsPolynomial());
          if (l.c != TValue(0)) {
            t.tp(l.index).base = term_bases::MakeLnAbsC<TValue>(l.c);
            f1.AddTermUnsafe(-t);
            t.tp(l.index).base = pone;
          } else {
            t.tp(l.index).base = term_bases::MakeLnAbs<TValue>();
            f1.AddTermUnsafe(-t);
            t.tp(l.index).base = pone;
          }
        }
        break;
      default:
        assert(false);
    }
  }
  f1.Compress();

  // Adjust power in all terms to avoid unsupported substitution
  MVFunction<TValue, dim> f2, sfn = ToFunction<TValue, dim>(sf.vn),
                              sfd = ToFunction<TValue, dim>(sf.vd);
  for (auto t : f1.terms) {
    int p = t.tp(index).power;
    t *= PowS(sf.a, p);
    f2 +=
        MVFunction<TValue, dim>(t) * PowU(sfn, p - minp) * PowU(sfd, maxp - p);
  }
  auto fd = PowU(Factorized<TValue>(TValue(1), {}, sf.vn), -minp) *
            PowU(Factorized<TValue>(TValue(1), {}, sf.vd), maxp);
  auto ff = FactorizedFunction<TValue, dim>(fd, f2);
  ff.Compress(true);
  return ff.f2;
}
}  // namespace ext_polynomial
}  // namespace calculus
