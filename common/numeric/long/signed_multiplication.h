#pragma once

#include "common/numeric/long/signed.h"
#include "common/numeric/long/unsigned_multiplication.h"

namespace numeric {
namespace nlong {
inline Signed MultBase(const Signed& a, const Signed& b) {
  return Signed(a.Sign() == b.Sign(),
                MultBase(a.GetUnsigned(), b.GetUnsigned()));
}

template <unsigned maxn = (1u << 16)>
inline Signed MultFFT(const Signed& a, const Signed& b) {
  return Signed(a.Sign() == b.Sign(),
                MultFFT<maxn>(a.GetUnsigned(), b.GetUnsigned()));
}

inline Signed MultAuto(const Signed& a, const Signed& b) {
  return Signed(a.Sign() == b.Sign(),
                MultAuto(a.GetUnsigned(), b.GetUnsigned()));
}
}  // namespace nlong
}  // namespace numeric

inline LongSigned operator*(const LongSigned& l, const LongSigned& r) {
  return numeric::nlong::MultAuto(l, r);
}

inline LongSigned& operator*=(LongSigned& l, const LongSigned& r) {
  l = numeric::nlong::MultAuto(l, r);
  return l;
}
