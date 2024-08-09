#pragma once

#include "common/base.h"
#include "common/numeric/utils/usqrt.h"

namespace numeric {
constexpr uint64_t Triangle(uint64_t n) { return (n * (n + 1)) / 2; }

constexpr bool IsTraingle(uint64_t n) {
  const auto m = USqrt(2 * n);
  return m * (m + 1) == 2 * n;
}
}  // namespace numeric
