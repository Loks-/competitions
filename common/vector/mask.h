#pragma once

#include "common/base.h"

#include <vector>

namespace nvector {
constexpr std::vector<unsigned> MaskToVector(uint64_t mask) {
  std::vector<unsigned> v;
  for (unsigned i = 0; mask; ++i) {
    uint64_t b = (1ull << i);
    if (mask & b) {
      v.push_back(i);
      mask &= ~b;
    }
  }
  return v;
}
}  // namespace nvector
