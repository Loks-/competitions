#pragma once

#include "common/numeric/long/signed.h"
#include "common/numeric/long/unsigned/gcd.h"
#include "common/numeric/utils/gcd.h"

inline LongSigned GCD(const LongSigned& a, const LongSigned& b) {
  return LongSigned(GCD(a.GetUnsigned(), b.GetUnsigned()));
}

inline LongSigned GCDSafe(const LongSigned& a, const LongSigned& b) {
  return LongSigned(GCD(a.GetUnsigned(), b.GetUnsigned()));
}
