#pragma once

#include "common/base.h"

namespace numeric {
inline unsigned BitsParity(uint32_t u) { return __builtin_parity(u); }
inline unsigned BitsParity(uint64_t u) { return __builtin_parityll(u); }
}  // namespace numeric
