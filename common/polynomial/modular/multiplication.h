#pragma once

#include "common/modular.h"
#include "common/modular/static/convolution.h"
#include "common/modular/static/convolution_fft.h"
#include "common/polynomial/multiplication.h"
#include "common/polynomial/polynomial.h"

namespace polynomial {
template <uint64_t prime>
inline Polynomial<ModularPrime32<prime>> SqrFFT(
    const Polynomial<ModularPrime32<prime>>& a) {
  return Polynomial<ModularPrime32<prime>>(
      modular::mstatic::ConvolutionFFT<ModularPrime32<prime>>::Convolution(
          a.Data()));
}

template <uint64_t prime>
inline Polynomial<ModularPrime32<prime>> MultFFT(
    const Polynomial<ModularPrime32<prime>>& a,
    const Polynomial<ModularPrime32<prime>>& b) {
  return Polynomial<ModularPrime32<prime>>(
      modular::mstatic::ConvolutionFFT<ModularPrime32<prime>>::Convolution(
          a.Data(), b.Data()));
}

template <uint64_t prime>
inline Polynomial<ModularPrime32<prime>> Sqr(
    const Polynomial<ModularPrime32<prime>>& a) {
  return (a.Size() < 100) ? SqrBase(a) : SqrFFT<prime>(a);
}

template <uint64_t prime>
inline Polynomial<ModularPrime32<prime>> Mult(
    const Polynomial<ModularPrime32<prime>>& a,
    const Polynomial<ModularPrime32<prime>>& b) {
  return ((a.Size() < 100) || (b.Size() < 100)) ? MultBase(a, b)
                                                : MultFFT<prime>(a, b);
}

template <uint64_t prime>
inline Polynomial<ModularPrime32<prime>> operator*(
    const Polynomial<ModularPrime32<prime>>& l,
    const Polynomial<ModularPrime32<prime>>& r) {
  return Mult<prime>(l, r);
}
}  // namespace polynomial
