#pragma once

#include "common/base.h"

namespace numeric {
constexpr unsigned BitsParity(uint32_t u) { return __builtin_parity(u); }
constexpr unsigned BitsParity(uint64_t u) { return __builtin_parityll(u); }
}  // namespace numeric
