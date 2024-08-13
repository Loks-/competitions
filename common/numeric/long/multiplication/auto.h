#pragma once

#include "common/numeric/long/multiplication/base.h"
#include "common/numeric/long/multiplication/fft.h"
#include "common/numeric/long/unsigned.h"

namespace numeric {
namespace nlong {
namespace multiplication {
inline Unsigned SqrAuto(const Unsigned& a) {
  return (a.Size() < 100) ? SqrBase(a) : SqrFFT(a);
}

inline Unsigned MultAuto(const Unsigned& a, const Unsigned& b) {
  return ((a.Size() < 100) || (b.Size() < 100)) ? MultBase(a, b)
                                                : MultFFT(a, b);
}
}  // namespace multiplication
}  // namespace nlong
}  // namespace numeric
