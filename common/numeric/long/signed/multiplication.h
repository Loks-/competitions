#pragma once

#include "common/numeric/long/multiplication/base.h"
#include "common/numeric/long/multiplication/fft.h"
#include "common/numeric/long/signed.h"
#include "common/numeric/long/unsigned/multiplication.h"

namespace numeric {
namespace nlong {
namespace multiplication {
constexpr Signed SqrBase(const Signed& a) {
  return Signed(SqrBase(a.GetUnsigned()));
}

constexpr Signed MultBase(const Signed& a, const Signed& b) {
  return Signed(a.Sign() == b.Sign(),
                MultBase(a.GetUnsigned(), b.GetUnsigned()));
}

inline Signed SqrFFT(const Signed& a) {
  return Signed(SqrFFT(a.GetUnsigned()));
}

inline Signed MultFFT(const Signed& a, const Signed& b) {
  return Signed(a.Sign() == b.Sign(),
                MultFFT(a.GetUnsigned(), b.GetUnsigned()));
}
}  // namespace multiplication

inline Signed Sqr(const Signed& a) { return Signed(Sqr(a.GetUnsigned())); }

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
