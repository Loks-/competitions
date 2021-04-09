#pragma once

#include "common/base.h"

namespace numeric {
// Returns one plus the index of the least significant 1-bit of x, or if x is
// zero, returns zero.
inline unsigned FirstBit(uint32_t u) { return __builtin_ffs(u); }
inline unsigned FirstBit(uint64_t u) { return __builtin_ffsll(u); }

// Returns the number of trailing 0-bits in x, starting at the least significant
// bit position. If x is 0, the result is undefined.
inline unsigned Lowest0Bits(uint32_t u) { return __builtin_ctz(u); }
inline unsigned Lowest0Bits(uint64_t u) { return __builtin_ctzll(u); }
}  // namespace numeric
