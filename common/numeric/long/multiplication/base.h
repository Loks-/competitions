#pragma once

#include "common/numeric/long/unsigned.h"

namespace numeric {
namespace nlong {
namespace multiplication {
constexpr Unsigned MultBase(const Unsigned& a, const Unsigned& b) {
  Unsigned r;
  for (auto it = b.end(); it != b.begin();) {
    r.ShiftBlocksRight(1);
    r += a * *(--it);
  }
  return r;
}

constexpr Unsigned SqrBase(const Unsigned& a) { return MultBase(a, a); }
}  // namespace multiplication
}  // namespace nlong
}  // namespace numeric
