#pragma once

#include "common/numeric/long/unsigned.h"

namespace numeric {
namespace nlong {
Unsigned MultBase(const Unsigned& a, const Unsigned& b) {
  Unsigned r;
  for (auto it = b.end(); it != b.begin();) {
    r.ShiftBlocksRight(1);
    r += a * *(--it);
  }
  return r;
}
}  // namespace nlong
}  // namespace numeric
