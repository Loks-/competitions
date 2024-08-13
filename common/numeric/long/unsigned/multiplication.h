#pragma once

#include "common/numeric/long/multiplication/auto.h"
#include "common/numeric/long/unsigned.h"

namespace numeric {
namespace nlong {
inline Unsigned Sqr(const Unsigned& a) { return multiplication::SqrAuto(a); }

inline Unsigned Mult(const Unsigned& a, const Unsigned& b) {
  return multiplication::MultAuto(a, b);
}

inline Unsigned operator*(const Unsigned& l, const Unsigned& r) {
  return Mult(l, r);
}

inline Unsigned& operator*=(Unsigned& l, const Unsigned& r) {
  l = Mult(l, r);
  return l;
}
}  // namespace nlong
}  // namespace numeric
