#pragma once

#include "common/numeric/long/signed.h"
#include "common/numeric/long/unsigned/multiplication.h"

namespace numeric {
namespace nlong {
constexpr Signed SqrBase(const Signed& a) {
  return Signed(SqrBase(a.GetUnsigned()));
}

constexpr Signed MultBase(const Signed& a, const Signed& b) {
  return Signed(a.Sign() == b.Sign(),
                MultBase(a.GetUnsigned(), b.GetUnsigned()));
}

template <unsigned log2_maxn = 16>
inline Signed SqrFFT(const Signed& a) {
  return Signed(SqrFFT<log2_maxn>(a.GetUnsigned()));
}

template <unsigned log2_maxn = 16>
inline Signed MultFFT(const Signed& a, const Signed& b) {
  return Signed(a.Sign() == b.Sign(),
                MultFFT<log2_maxn>(a.GetUnsigned(), b.GetUnsigned()));
}

template <unsigned log2_maxn = 16>
inline Signed Sqr(const Signed& a) {
  return Signed(Sqr<log2_maxn>(a.GetUnsigned()));
}

template <unsigned log2_maxn = 16>
inline Signed Mult(const Signed& a, const Signed& b) {
  return Signed(a.Sign() == b.Sign(),
                Mult<log2_maxn>(a.GetUnsigned(), b.GetUnsigned()));
}

inline Signed operator*(const Signed& l, const Signed& r) {
  return Mult<16>(l, r);
}

inline Signed& operator*=(Signed& l, const Signed& r) {
  l = Mult<16>(l, r);
  return l;
}
}  // namespace nlong
}  // namespace numeric
