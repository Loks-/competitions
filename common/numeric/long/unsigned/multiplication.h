#pragma once

#include "common/numeric/long/unsigned/multiplication_base.h"
#include "common/numeric/long/unsigned/multiplication_fft.h"

#include <vector>

namespace numeric {
namespace nlong {
template <unsigned log2_maxn = 16>
inline Unsigned Sqr(const Unsigned& a) {
  return (a.Size() < 100) ? SqrBase(a) : SqrFFT<log2_maxn>(a);
}

template <unsigned log2_maxn = 16>
inline Unsigned Mult(const Unsigned& a, const Unsigned& b) {
  return ((a.Size() < 100) || (b.Size() < 100)) ? MultBase(a, b)
                                                : MultFFT<log2_maxn>(a, b);
}

inline Unsigned operator*(const Unsigned& l, const Unsigned& r) {
  return Mult<16>(l, r);
}

inline Unsigned& operator*=(Unsigned& l, const Unsigned& r) {
  l = Mult<16>(l, r);
  return l;
}
}  // namespace nlong
}  // namespace numeric
