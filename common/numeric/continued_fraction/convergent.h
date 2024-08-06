#pragma once

#include "common/base.h"
#include "common/numeric/continued_fraction/continued_fraction.h"
#include "common/numeric/fraction.h"

namespace numeric {
namespace cf {
class CFConvergent {
 protected:
  ContinuedFraction cf;
  unsigned itc;
  int64_t n0, n1, d0, d1;

  constexpr void ResetIteratorI() {
    itc = 0;
    n0 = 1;
    n1 = 0;
    d0 = 0;
    d1 = 1;
  }

 public:
  constexpr void ResetIterator() {
    ResetIteratorI();
    Next();
  }

  constexpr explicit CFConvergent(const ContinuedFraction& continued_fraction)
      : cf(continued_fraction) {
    ResetIterator();
  }

  constexpr bool End() const { return itc > cf.Size(); }

  constexpr TIFraction Get() const { return TIFraction(n0, d0); }

  constexpr CFConvergent& Next() {
    if (itc < cf.Size()) {
      int64_t nn = cf(itc) * n0 + n1;
      n1 = n0;
      n0 = nn;
      int64_t dn = cf(itc) * d0 + d1;
      d1 = d0;
      d0 = dn;
    }
    ++itc;
    return *this;
  }

  constexpr TIFraction operator*() const { return Get(); }

  constexpr CFConvergent& operator++() { return Next(); }
};
}  // namespace cf
}  // namespace numeric
