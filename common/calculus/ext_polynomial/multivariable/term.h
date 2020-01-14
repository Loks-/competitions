#pragma once

#include "common/calculus/ext_polynomial/term.h"
#include "common/calculus/ext_polynomial/term_base.h"
#include "common/calculus/multivariable/point.h"
#include <array>

namespace calculus {
namespace ext_polynomial {
namespace multivariable {
template <class TValue>
using SVTerm = calculus::ext_polynomial::Term<TValue>;

template <class TValue, unsigned _dim>
using TermMV = std::array<PTermBase<TValue>, _dim>;

template <class TValue, unsigned _dim>
class Term {
 public:
  using TPoint = calculus::multivariable::Point<TValue, _dim>;
  using TTermBase = PTermBase<TValue>;
  using TTermSV = SVTerm<TValue>;
  using TTermMV = TermMV<TValue, _dim>;
  static const unsigned dim = _dim;

  TValue a;
  TTermMV b;

  Term(const TValue& _a, const TTermMV& _b) : a(_a), b(_b) {}
  Term(const TTermSV& t) {
    static_assert(dim == 1);
    a = t.a;
    b[0] = t.b;
  }

  TValue Get(const TPoint& p) const {
    TValue r = a;
    for (unsigned i = 0; i < dim; ++i) r *= b[i]->Get(p(i));
    return r;
  }
};
}  // namespace multivariable
}  // namespace ext_polynomial
}  // namespace calculus
