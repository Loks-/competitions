#pragma once

#include "common/modular.h"
#include "common/modular/static/convolution.h"
#include "common/polynomial/multiplication.h"
#include "common/polynomial/polynomial.h"

namespace polynomial {
template <uint64_t prime, unsigned maxn = (1u << 16)>
inline Polynomial<TModular_P32<prime>> SqrFFT(
    const Polynomial<TModular_P32<prime>>& a) {
  return Polynomial<TModular_P32<prime>>(
      modular::mstatic::ConvolutionFFT<TModular_P32<prime>, maxn>(a.Data()));
}

template <uint64_t prime, unsigned maxn = (1u << 16)>
inline Polynomial<TModular_P32<prime>> MultFFT(
    const Polynomial<TModular_P32<prime>>& a,
    const Polynomial<TModular_P32<prime>>& b) {
  return Polynomial<TModular_P32<prime>>(
      modular::mstatic::ConvolutionFFT<TModular_P32<prime>, maxn>(a.Data(),
                                                                  b.Data()));
}

template <uint64_t prime, unsigned maxn = (1u << 16)>
inline Polynomial<TModular_P32<prime>> Sqr(
    const Polynomial<TModular_P32<prime>>& a) {
  return (a.Size() < 100) ? SqrBase(a) : SqrFFT<prime, maxn>(a);
}

template <uint64_t prime, unsigned maxn = (1u << 16)>
inline Polynomial<TModular_P32<prime>> Mult(
    const Polynomial<TModular_P32<prime>>& a,
    const Polynomial<TModular_P32<prime>>& b) {
  return ((a.Size() < 100) || (b.Size() < 100)) ? MultBase(a, b)
                                                : MultFFT<prime, maxn>(a, b);
}

template <uint64_t prime>
inline Polynomial<TModular_P32<prime>> operator*(
    const Polynomial<TModular_P32<prime>>& l,
    const Polynomial<TModular_P32<prime>>& r) {
  return Mult<prime>(l, r);
}
}  // namespace polynomial
