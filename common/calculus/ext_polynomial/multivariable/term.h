#pragma once

#include "common/calculus/ext_polynomial/multivariable/term_power.h"
#include "common/calculus/ext_polynomial/term.h"
#include "common/calculus/multivariable/point.h"
#include <array>

namespace calculus {
namespace ext_polynomial {
namespace multivariable {
template <class TValue>
using SVTerm = calculus::ext_polynomial::Term<TValue>;

template <class TValue, unsigned _dim>
class Term {
 public:
  using TPoint = calculus::multivariable::Point<TValue, _dim>;
  using TTermPower = TermPower<TValue, _dim>;
  using TTermSV = SVTerm<TValue>;
  using TSelf = Term<TValue, _dim>;
  static const unsigned dim = _dim;

  TValue a;
  TTermPower tp;

  Term(const TValue& _a) : a(_a) {}
  Term(const TValue& _a, const TTermPower& _tp) : a(_a), tp(_tp) {}
  Term(const TTermSV& svt) {
    static_assert(dim == 1);
    a = svt.a;
    tp.terms[0] = svt.tp;
  }

  TTermSV ToSVTerm() const {
    static_assert((dim == 0) || (dim == 1));
    return TTermSV(a, (dim == 0) ? SVTermPower<TValue>() : tp(0));
  }

  TValue Get(const TPoint& p) const {
    TValue r = a;
    for (unsigned i = 0; i < dim; ++i) r *= tp(i)->Get(p(i));
    return r;
  }

  TSelf operator-() const { return TSelf(-a, tp); }

  bool UnusedVariable(unsigned index) const { return tp.UnusedVariable(); }
};
}  // namespace multivariable
}  // namespace ext_polynomial
}  // namespace calculus
