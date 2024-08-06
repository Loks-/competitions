#pragma once

#include "common/numeric/long/signed.h"
#include "common/numeric/long/unsigned/gcd.h"

namespace numeric {
namespace nlong {
constexpr Signed GCD(const Signed& a, const Signed& b) {
  return Signed(GCD(a.GetUnsigned(), b.GetUnsigned()));
}

constexpr Signed GCDSafe(const Signed& a, const Signed& b) {
  return Signed(GCD(a.GetUnsigned(), b.GetUnsigned()));
}
}  // namespace nlong
}  // namespace numeric
