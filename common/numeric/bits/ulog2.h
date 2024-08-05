#pragma once

#include "common/base.h"

namespace numeric {
constexpr unsigned ULog2(uint32_t u) { return 31 - __builtin_clz(u | 1); }
constexpr unsigned ULog2(uint64_t u) { return 63 - __builtin_clzll(u | 1); }
}  // namespace numeric
