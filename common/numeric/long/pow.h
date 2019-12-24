#pragma once

#include "common/base.h"
#include "common/numeric/long/unsigned.h"

namespace numeric {
namespace nlong {
template <class T>
inline Unsigned PowU(T x, unsigned pow) {
  Unsigned r(1u);
  for (unsigned i = 0; i < pow; ++i) r *= x;
  return r;
}
}  // namespace nlong
}  // namespace numeric
