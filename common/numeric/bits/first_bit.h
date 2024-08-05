#pragma once

#include "common/base.h"

namespace numeric {
// Returns one plus the index of the least significant 1-bit of x, or if x is
// zero, returns zero.
constexpr unsigned FirstBit(uint32_t u) { return __builtin_ffs(u); }
constexpr unsigned FirstBit(uint64_t u) { return __builtin_ffsll(u); }

// Returns the number of trailing 0-bits in x, starting at the least significant
// bit position. If x is 0, the result is undefined.
constexpr unsigned Lowest0Bits(uint32_t u) { return __builtin_ctz(u); }
constexpr unsigned Lowest0Bits(uint64_t u) { return __builtin_ctzll(u); }

// Returns the number of leading 0-bits in x, starting at the least significant
// bit position. If x is 0, the result is undefined.
constexpr unsigned Highest0Bits(uint32_t u) { return __builtin_clz(u); }
constexpr unsigned Highest0Bits(uint64_t u) { return __builtin_clzll(u); }

// Returns the index of the most significant 1-bits in x. If x is 0, the result
// is undefined.
constexpr unsigned HighestBit(uint32_t u) { return 31 - Highest0Bits(u); }
constexpr unsigned HighestBit(uint64_t u) { return 63 - Highest0Bits(u); }
}  // namespace numeric
