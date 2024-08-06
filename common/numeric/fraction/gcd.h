#pragma once

#include "common/numeric/fraction.h"
#include "common/numeric/utils/gcd.h"

template <class T>
constexpr numeric::Fraction<T> GCD(const numeric::Fraction<T>& x,
                                   const numeric::Fraction<T>& y) {
  return {GCD(x.GetN() * y.GetD(), x.GetD() * y.GetN()), x.GetD() * y.GetD()};
}

template <class T>
constexpr numeric::Fraction<T> GCDSafe(const numeric::Fraction<T>& x,
                                       const numeric::Fraction<T>& y) {
  return {GCDSafe(x.GetN() * y.GetD(), x.GetD() * y.GetN()),
          x.GetD() * y.GetD()};
}
