#pragma once

#include "common/numeric/long/signed.h"
#include "common/numeric/long/unsigned/multiplication.h"

namespace numeric {
namespace nlong {
constexpr Signed MultBase(const Signed& a, const Signed& b) {
  return Signed(a.Sign() == b.Sign(),
                MultBase(a.GetUnsigned(), b.GetUnsigned()));
}

template <unsigned maxn = (1u << 16)>
inline Signed MultFFT(const Signed& a, const Signed& b) {
  return Signed(a.Sign() == b.Sign(),
                MultFFT<maxn>(a.GetUnsigned(), b.GetUnsigned()));
}

inline Signed Mult(const Signed& a, const Signed& b) {
  return Signed(a.Sign() == b.Sign(), Mult(a.GetUnsigned(), b.GetUnsigned()));
}

inline Signed operator*(const Signed& l, const Signed& r) { return Mult(l, r); }

inline Signed& operator*=(Signed& l, const Signed& r) {
  l = Mult(l, r);
  return l;
}
}  // namespace nlong
}  // namespace numeric
