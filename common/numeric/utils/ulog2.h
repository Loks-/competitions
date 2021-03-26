#pragma once

#include "common/base.h"

inline unsigned ULog2(uint32_t u) { return 31 - __builtin_clz(u | 1); }
inline unsigned ULog2(uint64_t u) { return 63 - __builtin_clzll(u | 1); }
