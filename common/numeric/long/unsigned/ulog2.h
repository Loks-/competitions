#pragma once

#include "common/numeric/bits/ulog2.h"
#include "common/numeric/long/unsigned.h"

namespace numeric {
inline unsigned ULog2(const LongUnsigned& u) {
  const auto& v = u.Data();
  return v.empty() ? 0 : ULog2(v.back()) + (v.size() - 1) * u.BitsPerBlock();
}
}  //  namespace numeric
