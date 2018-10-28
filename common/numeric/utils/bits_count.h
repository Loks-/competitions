#pragma once

#include "../../base.h"

inline unsigned BitsCount(uint32_t u) { return __builtin_popcount(u); }
inline unsigned BitsCount(uint64_t u) { return __builtin_popcountll(u); }
