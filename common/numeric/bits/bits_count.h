#pragma once

#include "common/base.h"

namespace numeric {
constexpr unsigned BitsCount(uint32_t u) { return __builtin_popcount(u); }
constexpr unsigned BitsCount(uint64_t u) { return __builtin_popcountll(u); }
}  // namespace numeric
