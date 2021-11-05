#pragma once

#include "common/base.h"
#include "common/numeric/utils/usqrt.h"

namespace numeric {
inline uint64_t Triangle(uint64_t n) { return (n * (n + 1)) / 2; }

inline bool IsTraingle(uint64_t n) {
  auto m = USqrt(2 * n);
  return m * (m + 1) == 2 * n;
}
}  // namespace numeric
