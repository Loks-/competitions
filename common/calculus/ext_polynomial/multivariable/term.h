#pragma once

#include "common/calculus/ext_polynomial/term.h"
#include "common/calculus/ext_polynomial/term_base.h"
#include "common/calculus/ext_polynomial/term_bases/one.h"
#include "common/calculus/multivariable/point.h"
#include <array>

namespace calculus {
namespace ext_polynomial {
namespace multivariable {
template <class TValue, unsigned _dim>
using TermBase = std::array<PTermBase<TValue>, _dim>;

template <class TValue>
using SVTerm = calculus::ext_polynomial::Term<TValue>;

template <class TValue, unsigned _dim>
class Term {
 public:
  using TPoint = calculus::multivariable::Point<TValue, _dim>;
  using TTermBase = TermBase<TValue, _dim>;
  using TTermSV = SVTerm<TValue>;
  static const unsigned dim = _dim;

  TValue a;
  TTermBase b;

  Term(const TValue& _a, const TTermBase& _b) : a(_a), b(_b) {}
  Term(const TTermSV& t) {
    static_assert(dim == 1);
    a = t.a;
    b[0] = t.b;
  }

  TTermSV ToSVTerm() const {
    static_assert((dim == 0) || (dim == 1));
    return TTermSV(a, (dim == 0) ? term_bases::MakeOne<TValue>(0) : b[0]);
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
